#pragma once
#include <GLFW/glfw3.h>

struct GLFWwindow;

class Window
{
public:
	Window(const unsigned int width, const unsigned int height, const char* title);
	~Window();
	bool Open();
	GLFWwindow* GetWindow() const { return _window; }
	float GetAspectRatio() const { return (float)_width / _height; }
	unsigned int GetWidth() const { return _width; }
	unsigned int GetHeight() const { return _height; }
	bool SizeChanged();
private:
	GLFWwindow* _window;
	unsigned int _width;
	unsigned int _height;
	bool _sizeChanged;
	static void SetSize(GLFWwindow* window, int width, int height);
	static Window* _instance;
};

