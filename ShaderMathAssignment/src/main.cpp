#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Shader.h"
#include "FileManager.h"

void OnKeyEvent(GLFWwindow* window, const int key, const int scanCode, const int action, const int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);
}

int main()
{
	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(800	, 600, "GLFW Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//input
	glfwSetKeyCallback(window, OnKeyEvent);

	glewInit();

	//vertices
	float vertices[] =
	{
		-0.5f, -0.5f, 
		0.5f, -0.5f,
		0.0f, 0.5f
	};
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Draw triangle
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//Color
	float colors[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	//Draw triangle
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

	//Shader
	std::string vertexShader = utils::ReadFile("shaders/vertexShader.vert");
	printf(vertexShader.c_str());
	std::string fragmentShader = utils::ReadFile("shaders/fragmentShader.frag");
	printf(fragmentShader.c_str());
	Shader shader(vertexShader, fragmentShader);
	shader.Bind();
	
	while (!glfwWindowShouldClose(window))
	{		
		//Clear
		glfwSwapBuffers(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	

		glDrawArrays(GL_TRIANGLES, 0, 3);
	
		//Poll events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}