#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

void OnKeyEvent(GLFWwindow* window, const int key, const int scanCode, const int action, const int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);
}

int main()
{
	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(800 , 600, "GLFW Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//input
	glfwSetKeyCallback(window, OnKeyEvent);
	glewInit();
	
	//Quad
	float quadVertices[] =
	{
		-0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
		0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
		0.5f, 0.5f,        0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
		-0.5f, 0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
	};
	BufferLayout layout;
	layout.AddLayoutElement(2, GL_FLOAT, false, sizeof(float) * (2 + 3 + 2), 0);
	layout.AddLayoutElement(2, GL_FLOAT, false, sizeof(float) * (2 + 3 + 2),sizeof(float) * 2);
	layout.AddLayoutElement(2, GL_FLOAT, false, sizeof(float) * (2 + 3 + 2), sizeof(float) * (2 + 3));
	
	VertexBuffer quadBuffer(quadVertices, sizeof(float) * 7 * 4);
	quadBuffer.SetLayout(layout);
	VertexArray quadVao;
	quadVao.AddVertexBuffer(&quadBuffer);
	
	float triVertices[] =
	{
		-0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
		0.0f, 0.5f,        0.0f, 0.0f, 1.0f,    0.5f, 1.0f,
		0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f
	};
	VertexBuffer triBuffer(triVertices, sizeof(float) * 7 * 3);
	triBuffer.SetLayout(layout);
	VertexArray triVao;
	triVao.AddVertexBuffer(&triBuffer);

	//Shader
	const ShaderSource source = Shader::ParseShaderFile("shaders/Basic.shader");
	const Shader shader(source.VertexSource, source.FragmentSource);
	shader.Bind();
	
	shader.UploadUniformFloat("u_Scale", 0.5f);

	unsigned char textureColors[] =
	{
		255, 0,   0,   255,
		0,   255, 0,   255,
		0,   0,   255, 255,
		255, 255, 255, 255
	};
	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureColors);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//TODO: index buffers, mesh, material, renderable, renderer, transform
	while (!glfwWindowShouldClose(window))
	{		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	

		triVao.Bind();
		shader.UploadUniformFloat("u_Time", glfwGetTime());
		shader.UploadUniformVec2("u_Offset", -0.5f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		quadVao.Bind();	
		shader.UploadUniformFloat("u_Time", -glfwGetTime());
		shader.UploadUniformVec2("u_Offset", 0.5f, 0.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}