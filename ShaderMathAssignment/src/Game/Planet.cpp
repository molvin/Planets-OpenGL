#include "Planet.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/Renderer.h"
#include "PlanetSettings.h"
#include "../ImGUI/imgui.h"
#include "fstream"
#include "filesystem"


//----- Planet -----
Planet::Planet(std::string path)
{
	_path = path;
	Load();
}
Planet::Planet(PlanetSettings* settings)
{
	_settings = settings;
	GeneratePlanet();
}
void Planet::Render(Material& material)
{
	material.SetUniform("u_ElevationMinMax", glm::vec2(_elevation.Min, _elevation.Max));
	for (std::unique_ptr<PlanetFace>& face : _faces)
	{
		Renderer::Render(&material, face->GetMesh().GetVertexArray(), _transform.GetMatrix());
	}
}

void Planet::RenderGui()
{
	ImGui::Begin("Planet Settings");

	ImGui::Text("Transform");
	ImGui::InputFloat3("Position", &GetTransform()->Position[0]);
	glm::vec3 euler = GetTransform()->GetEuler();
	const bool changed = ImGui::SliderFloat3("Rotation", &euler[0], -180.0f, 180.0f);
	if(changed) GetTransform()->SetEuler(euler);
	ImGui::InputFloat3("Scale", &GetTransform()->Scale[0]);
	ImGui::Text("Settings");
	ImGui::InputInt("Resolution", &_settings->Resolution);
	ImGui::InputFloat("Radius", &_settings->Radius);
	ImGui::InputFloat("Strength", &_settings->Noise[0].Strength);
	ImGui::InputInt("Layer Count", &_settings->Noise[0].LayerCount);
	ImGui::InputFloat("Base Roughness", &_settings->Noise[0].BaseRoughness);
	ImGui::InputFloat("Roughness", &_settings->Noise[0].Roughness);
	ImGui::InputFloat("Persistence", &_settings->Noise[0].Persistence);
	ImGui::InputFloat("MinValue", &_settings->Noise[0].MinValue);
	ImGui::InputFloat3("Center", &_settings->Noise[0].Center.x);


	const bool regenerate = ImGui::Button("Regenerate");
	const bool save = ImGui::Button("Save to file");
	const bool load = ImGui::Button("Load from file");
	ImGui::End();

	if (regenerate)
		GeneratePlanet();
	if (save)
		Save();
	if(load)
		Load();
}

void Planet::Save()
{
	if (_path.empty())
		return;
	std::ofstream file(_path);
	file << _settings->Resolution << "\n";
	file << _settings->Radius << "\n";
	const int size = _settings->Noise.size();
	file << size << "\n";
	for(int i = 0; i < size; i++)
	{
		file << _settings->Noise[i].Enabled << "\n";
		file << _settings->Noise[i].UseFirstLayerAsMask << "\n";
		file << _settings->Noise[i].Strength << "\n";
		file << _settings->Noise[i].LayerCount << "\n";
		file << _settings->Noise[i].BaseRoughness << "\n";
		file << _settings->Noise[i].Roughness << "\n";
		file << _settings->Noise[i].Persistence << "\n";
		file << _settings->Noise[i].MinValue << "\n";
		file << _settings->Noise[i].Center.x << "\n";
		file << _settings->Noise[i].Center.y << "\n";
		file << _settings->Noise[i].Center.z << "\n";

	}
	file.close();
}

void Planet::Load()
{
	if (!std::filesystem::exists(_path))
	{
		_settings = new PlanetSettings();
		_settings->Noise.emplace_back();
		Save();
	}
	else
	{
		_settings = new PlanetSettings();
		std::ifstream file(_path);
		if(!file)
		{
			printf("Failed to read file at: %s", _path.c_str());
		}
		file >> _settings->Resolution;
		file >> _settings->Radius;
		int size = 0;
		file >> size;
		_settings->Noise.resize(size);
		for (int i = 0; i < size; i++)
		{
			file >> _settings->Noise[i].Enabled;
			file >> _settings->Noise[i].UseFirstLayerAsMask;
			file >> _settings->Noise[i].Strength;
			file >> _settings->Noise[i].LayerCount;
			file >> _settings->Noise[i].BaseRoughness;
			file >> _settings->Noise[i].Roughness;
			file >> _settings->Noise[i].Persistence;
			file >> _settings->Noise[i].MinValue;
			file >> _settings->Noise[i].Center.x;
			file >> _settings->Noise[i].Center.y;
			file >> _settings->Noise[i].Center.z;
		}

		file.close();
	}	
	GeneratePlanet();
}

void Planet::GeneratePlanet()
{
	glm::vec3 directions[] =
	{
		glm::vec3(0.0f, 1.0f, 0.0f), //up
		glm::vec3(0.0f, -1.0f, 0.0f), //down
		glm::vec3(1.0f, 0.0f, 0.0f), //right
		glm::vec3(-1.0f, 0.0f, 0.0f), //left
		glm::vec3(0.0f, 0.0f, 1.0f), //forward
		glm::vec3(0.0f, 0.0f, -1.0f), //back
	};

	_elevation.Reset(1000000, -100000);
	for (int i = 0; i < 6; i++)
	{
		_faces[i] = std::make_unique<PlanetFace>(_settings->Resolution, directions[i], *_settings, _elevation);
	}

	printf("Elevation: min: %f, max: %f \n", _elevation.Min, _elevation.Max);
}

//----- Planet Face -----
PlanetFace::PlanetFace(const int resolution, const glm::vec3& localUp, PlanetSettings& settings, MinMaxFloat& elevation)
{
	struct Vertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(1.0f);
	};

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	const int vertexCount = resolution * resolution;
	const int indexCount = (resolution - 1) * (resolution - 1) * 6;
	vertices.resize(vertexCount);
	indices.resize(indexCount);

	const glm::vec3 axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
	const glm::vec3 axisB = glm::cross(localUp, axisA);

	for (int y = 0, vi = 0, ti = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++, vi++)
		{
			const glm::vec2 factor = glm::vec2(x, y) / (float)(resolution - 1);
			glm::vec3 pointOnCube = localUp + (factor.x - 0.5f) * 2.0f * axisA + (factor.y - 0.5f) * 2.0f * axisB;
			glm::vec3 pointOnSphere = glm::normalize(pointOnCube);
			glm::vec3 pointOnPlanet = settings.CalculatePointOnPlanet(pointOnSphere, elevation);

			vertices[vi] = {pointOnPlanet, glm::vec3(1.0f)};
			if (x == resolution - 1 || y == resolution - 1) continue;

			indices[ti] = vi;
			indices[ti + 1] = vi + resolution + 1;
			indices[ti + 2] = vi + resolution;

			indices[ti + 3] = vi;
			indices[ti + 4] = vi + 1;
			indices[ti + 5] = vi + resolution + 1;
			ti += 6;
		}
	}
	for (int i = 0; i < indexCount; i += 3)
	{
		//Triangle
		Vertex* v1 = &vertices[indices[i]];
		Vertex* v2 = &vertices[indices[i + 1]];
		Vertex* v3 = &vertices[indices[i + 2]];

		glm::vec3 normal = glm::normalize(glm::cross(v2->position - v1->position, v3->position - v1->position));
		v1->normal += normal;
		v2->normal += normal;
		v3->normal += normal;
	}
	for (int i = 0; i < vertexCount; i++)
		vertices[i].normal = glm::normalize(vertices[i].normal);

	const int vertexSize = sizeof(float) * 6;
	std::vector<LayoutElement> layout =
	{
		{3, GL_FLOAT, false, vertexSize, 0},
		{3, GL_FLOAT, false, vertexSize, sizeof(float) * 3},
	};
	_mesh = std::make_unique<Mesh>(&vertices[0].position[0], vertexSize, vertexCount, &indices[0], indexCount, layout);
}
