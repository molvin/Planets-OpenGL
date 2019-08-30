#include "Window.h"
#include <cstdio>
#include <GLFW/glfw3.h>

Window::Window(const unsigned width, const unsigned height, const char* title)
{
	if (!glfwInit())
	{
		printf("Failed to init glfw");
		return;
	}

	_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!_window)
	{
		printf("Failed to create window");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window);
	_width = width;
	_height = height;
}

Window::~Window()
{
	glfwDestroyWindow(_window);
}
bool Window::Open()
{
	return !glfwWindowShouldClose(_window);
}
