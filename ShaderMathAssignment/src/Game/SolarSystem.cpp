#include "SolarSystem.h"
#include "../ImGUI/imgui.h"
#include <fstream>
#include "../Application/Input.h"
#include "Camera.h"
#include "../Application/Window.h"

SolarSystem::SolarSystem()
{
	_path.reserve(100);
	_planets.reserve(10);

	_godLight.Color = glm::vec3(1.0f);
	_godLight.Intensity = 0.0f;
	_godLight.Direction = glm::normalize(glm::vec3(-1.0f));
	_ambientLight.Color = glm::vec3(1.0f);
	_ambientLight.Intensity = 0.05f;	
}

void SolarSystem::Update(Camera& camera, Window& window)
{
	if (Input::GetKeyDown(GLFW_KEY_F))
		_hideUi = !_hideUi;

	for(auto& planet : _planets)
	{
		float intersectDist = 0.0f;
		glm::vec3 rayOrigin;
		glm::vec3 rayDirection;
		glm::vec2 mousePos = Input::GetMousePosition();
		ScreenPosToWorldRay(mousePos.x, mousePos.y, window.GetWidth(), window.GetHeight(), camera.GetViewMatrix(), window.GetProjectionMatrix(), rayOrigin, rayDirection);
		bool intersect = CheckIntersect(rayOrigin, rayDirection, glm::vec3(-planet.GetSettings().Radius), glm::vec3(planet.GetSettings().Radius), planet.GetTransform()->GetMatrix(), intersectDist);
		if (intersect && Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_1))
		{
			printf("SELECTING PLANET");
			_selectedPlanet = &planet;
		}
	}
}

void SolarSystem::Render(const glm::vec3& eyePosition)
{
	std::vector<PointLight*> lights;
	for (auto& planet : _planets)
		lights.push_back(&planet.LightSource);
	if(_shadowBuffer != nullptr) _shadowBuffer->Bind(1);
	for (auto& planet : _planets)
		planet.Render(_godLight, _ambientLight, lights, eyePosition);
}

void SolarSystem::DrawGui()
{
	if (_hideUi) return;
	ImGui::Begin("Solar System (press F to toggle)");

	ImGui::Text("God Light");
	ImGui::SliderFloat3("Direction", &_godLight.Direction[0], -1.0f, 1.0f);
	ImGui::InputFloat("Intensity", &_godLight.Intensity);
	ImGui::ColorEdit3("Color", &_godLight.Color[0]);
	ImGui::Text("Ambient Light");
	ImGui::InputFloat("Ambiance", &_ambientLight.Intensity);

	ImGui::Text("Save/Load/Create");
	
	ImGui::InputText("Path", &_path[0], 1000);

	const bool createPlanet = ImGui::Button("Create Planet");
	const bool save = ImGui::Button("Save Scene");
	const bool load = ImGui::Button("Load Scene");

	if (createPlanet) CreatePlanet();
	if (save) Save();
	if (load) Load();
	
	ImGui::End();

	const int size = _planets.size();
	
	for (int i = size - 1; i >= 0; i--)
	{
		if (&_planets[i] != _selectedPlanet)
			continue;
		bool destroy = false;
		_planets[i].DrawGui(destroy);
		if (destroy) _planets.erase(_planets.begin() + i);
	}
	
	
}

void SolarSystem::SetShadowBuffer(const Texture* shadowBuffer)
{
	_shadowBuffer = shadowBuffer;
}

void SolarSystem::CreatePlanet()
{
	_planets.emplace_back();
	Planet& planet = _planets[_planets.size() - 1];
	planet.Id = _planetCounter++;
	planet.GeneratePlanet();	
}
void SolarSystem::Save()
{
	if (_path.empty())
		return;
	std::ofstream file(_path);

	const int size = _planets.size();
	file << size << "\n";
	for(int i = 0; i < size; i++)
	{
		Transform* transform = _planets[i].GetTransform();
		file << transform->Position.x << "\n";
		file << transform->Position.y << "\n";
		file << transform->Position.z << "\n";

		file << transform->Scale.x << "\n";
		file << transform->Scale.y << "\n";
		file << transform->Scale.z << "\n";

		file << transform->Rotation.x << "\n";
		file << transform->Rotation.y << "\n";
		file << transform->Rotation.z << "\n";
		file << transform->Rotation.w << "\n";		
		
		PlanetSettings settings = _planets[i].GetSettings();
		file << settings.Resolution << "\n";
		file << settings.Radius << "\n";
		const int settingsSize = settings.Noise.size();
		file << settingsSize << "\n";
		for (int j = 0; j < settingsSize; j++)
		{
			file << settings.Noise[j].Enabled << "\n";
			file << settings.Noise[j].UseFirstLayerAsMask << "\n";
			file << settings.Noise[j].Strength << "\n";
			file << settings.Noise[j].LayerCount << "\n";
			file << settings.Noise[j].BaseRoughness << "\n";
			file << settings.Noise[j].Roughness << "\n";
			file << settings.Noise[j].Persistence << "\n";
			file << settings.Noise[j].MinValue << "\n";
			file << settings.Noise[j].Center.x << "\n";
			file << settings.Noise[j].Center.y << "\n";
			file << settings.Noise[j].Center.z << "\n";
		}

		file << _planets[i].LightSource.Radius << "\n";
		file << _planets[i].LightSource.Color.r << "\n";
		file << _planets[i].LightSource.Color.g << "\n";
		file << _planets[i].LightSource.Color.b << "\n";
		file << _planets[i].LightSource.Intensity << "\n";
		

		file << _planets[i].Specular << "\n";
		
		std::list<ImGradientMark*> marks = _planets[i].Gradient.getMarks();
		const int markCount = marks.size();
		file << markCount << "\n";
		for(const auto& mark : marks)
		{
			file << mark->position << "\n";
			for (int j = 0; j < 4; j++)
				file << mark->color[j] << "\n";
		}
	}
	
	file.close();
}
void SolarSystem::Load()
{
	if (_path.empty())
		return;

	_planets.clear();
	_planetCounter = 0;

	std::ifstream file(_path);
	if (!file)
	{
		printf("Failed to read file at: %s", _path.c_str());
		return;
	}
	int size = 0;
	file >> size;
	
	for(int i = 0; i < size; i++)
	{
		_planets.emplace_back();
		_planets[i].Id = _planetCounter + i;
		
		Transform* transform = _planets[i].GetTransform();
		file >> transform->Position.x;
		file >> transform->Position.y;
		file >> transform->Position.z;
			 
		file >> transform->Scale.x;
		file >> transform->Scale.y;
		file >> transform->Scale.z;
			 
		file >> transform->Rotation.x;
		file >> transform->Rotation.y;
		file >> transform->Rotation.z;
		file >> transform->Rotation.w;		
		
		PlanetSettings settings;
		file >> settings.Resolution;
		file >> settings.Radius;
		int settingsSize = 0;
		file >> settingsSize;
		for(int j = 0; j < settingsSize; j++)
		{
			settings.Noise.emplace_back();
			file >> settings.Noise[j].Enabled;
			file >> settings.Noise[j].UseFirstLayerAsMask;
			file >> settings.Noise[j].Strength;
			file >> settings.Noise[j].LayerCount;
			file >> settings.Noise[j].BaseRoughness;
			file >> settings.Noise[j].Roughness;
			file >> settings.Noise[j].Persistence;
			file >> settings.Noise[j].MinValue;
			file >> settings.Noise[j].Center.x;
			file >> settings.Noise[j].Center.y;
			file >> settings.Noise[j].Center.z;
		}

		file >> _planets[i].LightSource.Radius;
		file >> _planets[i].LightSource.Color.r;
		file >> _planets[i].LightSource.Color.g;
		file >> _planets[i].LightSource.Color.b;
		file >> _planets[i].LightSource.Intensity;

		file >> _planets[i].Specular;
		
		int markCount = 0;
		file >> markCount;
		_planets[i].Gradient.getMarks().clear();
		for(int j = 0; j < markCount; j++)
		{
			float position = 0;
			ImColor color;

			file >> position;
			file >> color.Value.x;
			file >> color.Value.y;
			file >> color.Value.z;
			file >> color.Value.w;

			_planets[i].Gradient.addMark(position, color);
		}
		
		_planets[i].SetSettings(settings);
		_planets[i].GeneratePlanet();
	}
	_planetCounter += size;
	
}

bool SolarSystem::CheckIntersect(glm::vec3 origin, glm::vec3 rayDir, glm::vec3 min, glm::vec3 max, glm::mat4 model,	float& intersectDistance)
{
	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 obbPositionWorld(model[3].x, model[3].y, model[3].z);

	glm::vec3 delta = obbPositionWorld - origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(model[0].x, model[0].y, model[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(rayDir, xaxis);

		if (fabs(f) > 0.001f) { // Standard case

			float t1 = (e + min.x) / f; // Intersection with the "left" plane
			float t2 = (e + max.x) / f; // Intersection with the "right" plane
											 // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

											 // We want t1 to represent the nearest intersection, 
											 // so if it's not the case, invert t1 and t2
			if (t1>t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + min.x > 0.0f || -e + max.x < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(model[1].x, model[1].y, model[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(rayDir, yaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + min.y) / f;
			float t2 = (e + max.y) / f;

			if (t1>t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + min.y > 0.0f || -e + max.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		glm::vec3 zaxis(model[2].x, model[2].y, model[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(rayDir, zaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + min.z) / f;
			float t2 = (e + max.z) / f;

			if (t1>t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + min.z > 0.0f || -e + max.z < 0.0f)
				return false;
		}
	}

	intersectDistance = tMin;
	return true;
	
}
void SolarSystem::ScreenPosToWorldRay(int mouseX, int mouseY, int screenWidth, int screenHeight, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::vec3& out_origin, glm::vec3& out_direction)
{

	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	glm::vec4 lRayStart_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f,
		0.0,
		1.0f
	);


	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	out_origin = glm::vec3(lRayStart_world);
	out_direction = glm::normalize(lRayDir_world);
}