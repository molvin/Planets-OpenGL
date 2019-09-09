#pragma once
#include "../Graphics/Mesh.h"
#include "../MinMaxFloat.h"

class PlanetSettings;

//TODO: handle memory leaks, smart pointers

class PlanetFace
{
public:
	PlanetFace(const int resolution, const glm::vec3 & localUp, PlanetSettings & settings, MinMaxFloat& elevation);
	~PlanetFace();
	const Mesh& GetMesh() const { return *_mesh; }
private:
	Mesh* _mesh;
};

class Planet
{
public:
	Planet(PlanetSettings& settings);
	~Planet();
	void Render(Material& material);
	void RenderGui();
	
private:
	void GeneratePlanet();

	PlanetFace* _faces[6] = {nullptr};
	Transform _transform;
	MinMaxFloat _elevation = MinMaxFloat(1000000, -100000000);
	PlanetSettings* _settings;
};

