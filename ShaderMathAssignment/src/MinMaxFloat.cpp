#include "MinMaxFloat.h"

MinMaxFloat::MinMaxFloat() : Min(0.0f), Max(0.0f)
{
}

MinMaxFloat::MinMaxFloat(float min, float max) : Min(min), Max(max)
{
}

void MinMaxFloat::AddValue(const float value)
{
	if (value < Min) Min = value;
	else if (value > Max) Max = value;
}

void MinMaxFloat::Reset(const float min, const float max)
{
	Min = min;
	Max = max;
}
