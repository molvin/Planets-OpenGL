#include "PlanetSettings.h"
#include <algorithm>
#include "../MinMaxFloat.h"

glm::vec3 PlanetSettings::CalculatePointOnPlanet(const glm::vec3& point, MinMaxFloat& elevationMinMax)
{

	float elevation = 0.0f;
	float firstLayerValue = 0.0f;
	if (!Noise.empty())
	{
		firstLayerValue = EvaluateNoise(point, Noise[0]);
		if (Noise[0].Enabled)
			elevation = firstLayerValue;
	}

	const int size = Noise.size();
	for (int i = 1; i < size; i++)
	{
		if (!Noise[i].Enabled) continue;
		float mask = Noise[i].UseFirstLayerAsMask ? firstLayerValue : 1.0f;
		elevation += EvaluateNoise(point, Noise[i]);
	}

	elevation = Radius * (1 + elevation);
	elevationMinMax.AddValue(elevation);
	return point * elevation;
}

float PlanetSettings::EvaluateNoise(const glm::vec3& point, const NoiseSettings& settings) const
{
	float noiseValue = 0.0f;
	float frequency = settings.BaseRoughness;
	float amplitude = 1.0f;

	for (int i = 0; i < settings.LayerCount; i++)
	{
		const glm::vec3 evaluationPoint = point * frequency + settings.Center;
		const float rawNoise = _perlin.GetValue(evaluationPoint.x, evaluationPoint.y, evaluationPoint.z);
		noiseValue += (rawNoise + 1) * 0.5f * amplitude;
		frequency *= settings.Roughness;
		amplitude *= settings.Persistence;
	}

	noiseValue = std::max(0.0f, noiseValue - settings.MinValue);
	return noiseValue * settings.Strength;
}
