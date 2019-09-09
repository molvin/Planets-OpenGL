#pragma once
struct MinMaxFloat
{
public:
	float Min = 0.0f;
	float Max = 0.0f;

	MinMaxFloat();
	MinMaxFloat(float min, float max);

	void AddValue(const float value);
	void Reset(const float min = 0, const float max = 0);
};

