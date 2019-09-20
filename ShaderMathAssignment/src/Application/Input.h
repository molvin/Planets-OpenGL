#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

#define MIN_MOUSE_DELTA 2.0f

class Input
{
public:
	static char _keys[];
	static char _mouseButtons[];
	
	static bool GetKeyDown(const int key) { return (_keys[key] & 8) > 0; }
	static bool GetKeyUp(const int key) { return (_keys[key] & 2) > 0; }
	static bool GetKey(const int key) { return (_keys[key] & 4) > 0; }

	static bool GetMouseButtonDown(const int button) { return (_mouseButtons[button] & 8) > 0; }
	static bool GetMouseButtonUp(const int button) { return (_mouseButtons[button] & 2) > 0; }
	static bool GetMouseButton(const int button) { return (_mouseButtons[button] & 4) > 0; }
	
	static glm::vec2 GetMousePosition() { return _currentMousePosition; }
	static glm::vec2 GetMouseDirection() { return glm::length(_lastMousePosition - _currentMousePosition) < MIN_MOUSE_DELTA ? glm::vec2(0.0f) : glm::normalize(_lastMousePosition - _currentMousePosition); }
	static float GetMouseDelta() { return glm::max(glm::length(_lastMousePosition - _currentMousePosition) - MIN_MOUSE_DELTA, 0.0f); }
	static void Init(GLFWwindow* window);
	static void Update();
private:
	static void OnKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void OnCursorMove(GLFWwindow* window, double x, double y);
	static void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
	static float MinMouseDelta;
	static glm::vec2 _lastMousePosition;
	static glm::vec2 _currentMousePosition;
};

