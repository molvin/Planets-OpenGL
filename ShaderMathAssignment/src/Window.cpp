#include "Window.h"
#include <cstdio>
#include <GLFW/glfw3.h>

Window* Window::_instance = nullptr;

Window::Window(const unsigned width, const unsigned height, const char* title)
{
	_instance = this;
	
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

	glfwSetWindowSizeCallback(_window, &Window::SetSize);
}

Window::~Window()
{
	glfwDestroyWindow(_window);
}
bool Window::Open()
{
	return !glfwWindowShouldClose(_window);
}

bool Window::SizeChanged()
{
	if (!_sizeChanged) return false;
	_sizeChanged = false;
	return true;
}

void Window::SetSize(GLFWwindow* window, int width, int height)
{
	if (_instance == nullptr) return;
	_instance->_sizeChanged = true;
	_instance->_width = width;
	_instance->_height = height;
	glViewport(0, 0, width, height);
}
