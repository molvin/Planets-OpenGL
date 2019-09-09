#pragma once
#include <glm/vec3.hpp>
#include "../Noise/FastNoise.h"
#include <vector>

struct MinMaxFloat;

struct NoiseSettings
{
	bool Enabled = true;
	bool UseFirstLayerAsMask = true;
	float Strength = .35f;
	int LayerCount = 5;
	float BaseRoughness = 1.7;
	float Roughness = 3.5;
	float Persistence = .65f;
	float MinValue = 1.21f;
	glm::vec3 Center = glm::vec3(0.0f);
};

class PlanetSettings
{
public:
	glm::vec3 CalculatePointOnPlanet(const glm::vec3& point, MinMaxFloat& elevationMinMax);

	int Resolution = 50;
	float Radius = 1.0f;
	std::vector<NoiseSettings> Noise;
private:
	float EvaluateNoise(const glm::vec3& point, const NoiseSettings& settings) const;
	FastNoise _noise;
};

