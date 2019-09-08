#pragma once
#include <glm/vec3.hpp>
#include "../Noise/FastNoise.h"
#include <vector>

struct NoiseSettings
{
	bool Enabled = true;
	bool UseFirstLayerAsMask = true;
	float Strength = .45f;
	int LayerCount = 5;
	float BaseRoughness = 8;
	float Roughness = 4;
	float Persistence = .65f;
	float MinValue = 1.1f;
	glm::vec3 Center = glm::vec3(0.0f);
};

class PlanetSettings
{
public:
	glm::vec3 CalculatePointOnPlanet(const glm::vec3& point);

	float Radius = 1.0f;
	std::vector<NoiseSettings> Noise;
private:
	float EvaluateNoise(const glm::vec3& point, const NoiseSettings& settings) const;
	FastNoise _noise;
};

