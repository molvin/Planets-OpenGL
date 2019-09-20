#pragma once
#include "../Graphics/Mesh.h"
#include "MinMaxFloat.h"
#include "PlanetSettings.h"
#include "../ImGUI/imgui_color_gradient.h"
#include "../Graphics/Material.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Light/AmbientLight.h"
#include "../Graphics/Light/DirectionalLight.h"
#include "../Graphics/Light/PointLight.h"
#include <vector>

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
	void Render(DirectionalLight& dirLight, AmbientLight& ambient, std::vector<PointLight*>& pointLights, const glm::vec3& eyePosition);
	void DrawGui(bool& destroy);
	void GeneratePlanet();
	const PlanetSettings& GetSettings() const { return _settings; }
	void SetSettings(const PlanetSettings settings) { _settings = settings; }
	Transform* GetTransform() { return &_transform; }
	unsigned int Id = 0;
	ImGradient Gradient;
	float Specular = 0.0f;
	PointLight LightSource;

private:
	void SetColors();

	std::unique_ptr<PlanetFace> _faces[6];
	Transform _transform;
	MinMaxFloat _elevation = MinMaxFloat(1000000.f, -100000000.f);
	PlanetSettings _settings;
	std::string _path;
	Texture* _texture = nullptr;
	Material _material;
};

