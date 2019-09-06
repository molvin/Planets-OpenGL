#pragma once
#include "Window.h"

class ImGuiRenderer
{
public:
	static void Init(GLFWwindow* window);
	static void Begin();
	static void End();
	static void Terminate();

};

