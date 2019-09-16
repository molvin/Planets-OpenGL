#pragma once
#include "../Graphics/Mesh.h"
#include "../MinMaxFloat.h"

class PlanetSettings;

//TODO: handle memory leaks, smart pointers

class PlanetFace
{
public:
	PlanetFace(const int resolution, const glm::vec3 & localUp, PlanetSettings & settings, MinMaxFloat& elevation);
	const Mesh& GetMesh() const { return *_mesh; }
private:
	std::unique_ptr<Mesh> _mesh;
};

class Planet
{
public:
	Planet(std::string path);
	Planet(PlanetSettings* settings);
	void Render(Material& material);
	void RenderGui();
	void Save();
	void Load();
	Transform* GetTransform() { return &_transform; }
	
private:
	void GeneratePlanet();

	std::unique_ptr<PlanetFace> _faces[6];
	Transform _transform;
	glm::vec3 _euler;	//TODO: this should be handled by transform
	MinMaxFloat _elevation = MinMaxFloat(1000000, -100000000);
	PlanetSettings* _settings;
	std::string _path;
};

