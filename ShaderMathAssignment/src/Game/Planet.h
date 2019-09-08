#pragma once
#include "../Graphics/Mesh.h"


class PlanetSettings;


class PlanetFace
{
public:
	PlanetFace(const int resolution, const glm::vec3 & localUp, PlanetSettings & settings);
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
	void Render(const Material& material);
	void RenderGui();
	
private:
	void GeneratePlanet();

	PlanetFace* _faces[6] = {nullptr};
	Transform _transform;
	PlanetSettings* _settings;
};

