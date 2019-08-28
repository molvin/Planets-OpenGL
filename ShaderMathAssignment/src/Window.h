#pragma once

struct GLFWwindow;

class Window
{
public:
	Window(const unsigned int width, const unsigned int height, const char* title);
	~Window();
	bool Open();
	GLFWwindow* GetWindow() const { return _window; }
private:
	GLFWwindow* _window;
};

