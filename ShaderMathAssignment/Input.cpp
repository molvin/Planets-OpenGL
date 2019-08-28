#include "Input.h"

char Input::keys[GLFW_KEY_LAST] = { 0 };

void Input::OnKeyEvent(GLFWwindow* window, const int key, const int scanCode, const int action, const int mods)
{
	if (action == GLFW_REPEAT) return;

	if (action == GLFW_PRESS)
		keys[key] |= 1;
	if (action == GLFW_RELEASE)
		keys[key] |= 2;
}
void Input::Init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, &Input::OnKeyEvent);
}
void Input::Update()
{
	for (char& key : keys)
	{
		if((key & 1) > 0)	//Press bit is set, clear press bit and set held bit
		{
			key |= 4;
			key &= ~1;
		}
		if ((key & 2) > 0)	//Release bit is set, clear release bit and clear held bit
		{
			key &= ~2;
			key &= ~4;
		}
	}
}
