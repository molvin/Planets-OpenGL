#include "Time.h"

std::chrono::steady_clock::time_point Time::_latest;
float Time::_deltaTime = 0.0f;

void Time::Init()
{
	_latest = std::chrono::high_resolution_clock::now();
	_deltaTime = 0.016f;
}
void Time::Tick()
{
	std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = now - _latest;
	_latest = now;
	_deltaTime = elapsed.count();
}
