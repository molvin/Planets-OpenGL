#include "Window.h"
#include <cstdio>
#include <GLFW/glfw3.h>
#include <glm/gtx/functions.hpp>
#include <glm/ext/matrix_clip_space.hpp>


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

	_projection = glm::perspective(glm::radians(60.0f), GetAspectRatio(), 0.1f, 1000.0f);

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
void Window::SetSize(GLFWwindow* window, int width, int height)
{
	if (_instance == nullptr) return;
	_instance->_width = width;
	_instance->_height = height;
	_instance->_projection = glm::perspective(glm::radians(60.0f), _instance->GetAspectRatio(), 0.1f, 1000.0f);
	glViewport(0, 0, width, height);
}
