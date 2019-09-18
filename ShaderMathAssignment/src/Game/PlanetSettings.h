#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include "noise/noise.h"

struct MinMaxFloat;

struct NoiseSettings
{
	bool Enabled = true;
	bool UseFirstLayerAsMask = true;
	float Strength = 0.15f;
	int LayerCount = 5;
	float BaseRoughness = 1.5;
	float Roughness = 0.35;
	float Persistence = .65f;
	float MinValue = 1.25f;
	glm::vec3 Center = glm::vec3(5.0f, 8.0f, 0.0f);
};

class PlanetSettings
{
public:
	PlanetSettings() = default;
	
	glm::vec3 CalculatePointOnPlanet(const glm::vec3& point, MinMaxFloat& elevationMinMax) const;

	int Resolution = 50;
	float Radius = 1.0f;
	std::vector<NoiseSettings> Noise;
	PlanetSettings& operator=(const PlanetSettings& other);
private:
	float EvaluateNoise(const glm::vec3& point, const NoiseSettings& settings) const;
	noise::module::Perlin _perlin;
};

