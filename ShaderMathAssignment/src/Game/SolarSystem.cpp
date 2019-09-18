#include "SolarSystem.h"
#include "../ImGUI/imgui.h"
#include <fstream>

SolarSystem::SolarSystem()
{
	_path.reserve(1000);
	_planets.reserve(10);
}

void SolarSystem::Render()
{
	for (auto& planet : _planets)
		planet.Render();
}

void SolarSystem::DrawGui()
{
	ImGui::Begin("Solar System");

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
		bool destroy = false;
		_planets[i].DrawGui(destroy);
		if (destroy) _planets.erase(_planets.begin() + i);
	}
	
	
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
		_planets[i].SetSettings(settings);
		_planets[i].GeneratePlanet();
	}
	_planetCounter += size;
	
}
