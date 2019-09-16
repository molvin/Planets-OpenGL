#pragma once
#include <chrono>

class Time
{
public:
	static void Init();
	static void Tick();
	static float GetDeltaTime() { return _deltaTime; } 
private:
	static std::chrono::steady_clock::time_point _latest;
	static float _deltaTime;
};

