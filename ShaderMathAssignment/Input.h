#pragma once
#include <GLFW/glfw3.h>

class Input
{
public:
	static char keys[];
	static bool GetKeyDown(const int key) { return (keys[key] & 1) > 0; }
	static bool GetKeyUp(const int key) { return (keys[key] & 2) > 0; }
	static bool GetKey(const int key) { return (keys[key] & 4) > 0; }
	static void Init(GLFWwindow* window);
	static void Update();
private:
	static void OnKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods);
};

