#pragma once

struct GLFWwindow;

class Window
{
public:
	Window(const unsigned int width, const unsigned int height, const char* title);
	~Window();
	bool Open();
	GLFWwindow* GetWindow() const { return _window; }
	float GetAspectRatio() { return (float)_width / (float)_height; }
	unsigned int GetWidth() { return _width; }
	unsigned int GetHeight() { return _width; }
private:
	GLFWwindow* _window;
	unsigned int _width;
	unsigned int _height;
};

