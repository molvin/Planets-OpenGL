#include "Input.h"
#include <cstdio>

char Input::_keys[GLFW_KEY_LAST] = { 0 };
char Input::_mouseButtons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
glm::vec2 Input::_lastMousePosition = glm::vec2(0.0f);
glm::vec2 Input::_currentMousePosition = glm::vec2(0.0f);

void Input::OnKeyEvent(GLFWwindow* window, const int key, const int scanCode, const int action, const int mods)
{
	if (action == GLFW_REPEAT) return;

	if (action == GLFW_PRESS)
		_keys[key] |= 1;
	if (action == GLFW_RELEASE)
		_keys[key] |= 2;
}
void Input::OnCursorMove(GLFWwindow* window, const double x, const double y)
{
	_lastMousePosition = _currentMousePosition;
	_currentMousePosition.x = x;
	_currentMousePosition.y = y;
}
void Input::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_REPEAT) return;
	
	if (action == GLFW_PRESS)
		_mouseButtons[button] |= 1;
	if (action == GLFW_RELEASE)
		_mouseButtons[button] |= 2;
}

void Input::Init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, &Input::OnKeyEvent);
	glfwSetCursorPosCallback(window, &Input::OnCursorMove);
	glfwSetMouseButtonCallback(window, &Input::OnMouseButton);
}
void Input::Update()
{
	for (char& key : _keys)
	{
		if ((key & 8) > 0)
		{
			key &= ~8;
		}
		
		if((key & 1) > 0)	//Press bit is set, clear press bit and set held bit
		{
			key |= 4;
			key |= 8;
			key &= ~1;
		}

		if ((key & 2) > 0)	//Release bit is set, clear release bit and clear held bit
		{
			key &= ~2;
			key &= ~4;
		}
	}

	for (char& button : _mouseButtons)
	{
		if ((button & 8) > 0)
		{
			button &= ~8;
		}
		
		if ((button & 1) > 0)	//Press bit is set, clear press bit and set held bit
		{
			button |= 4;
			button |= 8;
			button &= ~1;
		}
		if ((button & 2) > 0)	//Release bit is set, clear release bit and clear held bit
		{
			button &= ~2;
			button &= ~4;
		}
	}
}
