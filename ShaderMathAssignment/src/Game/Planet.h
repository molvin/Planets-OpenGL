#pragma once
#include "../Graphics/Mesh.h"
#include "MinMaxFloat.h"
#include "PlanetSettings.h"
#include "../ImGUI/imgui_color_gradient.h"
#include "../Graphics/Material.h"
#include "../Graphics/Texture.h"


//TODO: handle memory leak
//TODO: emit light
//TODO: gradient editing, material for each planet

class PlanetFace
{
public:
	PlanetFace(const int resolution, const glm::vec3 & localUp, const PlanetSettings& settings, MinMaxFloat& elevation);
	const Mesh& GetMesh() const { return *_mesh; }
private:
	std::unique_ptr<Mesh> _mesh;
};

class Planet
{
public:
	Planet();
	void Render();
	void DrawGui(bool& destroy);
	void GeneratePlanet();
	const PlanetSettings& GetSettings() const { return _settings; }
	void SetSettings(const PlanetSettings settings) { _settings = settings; }
	Transform* GetTransform() { return &_transform; }
	unsigned int Id;
private:
	void SetColors();

	std::unique_ptr<PlanetFace> _faces[6];
	Transform _transform;
	MinMaxFloat _elevation = MinMaxFloat(1000000.f, -100000000.f);
	PlanetSettings _settings;
	std::string _path;
	ImGradient _gradient;
	Texture* _texture;
	Material _material;
};

