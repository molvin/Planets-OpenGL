#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Texture.h"
#include <map>
#include "Mesh.h"

std::map<int, bool> keyMap;

void OnKeyEvent(GLFWwindow* window, const int key, const int scanCode, const int action, const int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);

	if (action == GLFW_PRESS) keyMap[key] = true;
	else if (action == GLFW_RELEASE) keyMap[key] = false;
}	
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,	const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}


int main()
{
	const int screenWidth = 1280;
	const int screenHeight = 720;

	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(screenWidth , screenHeight, "GLFW Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//input
	glfwSetKeyCallback(window, OnKeyEvent);
	glewInit();
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	//Buffer layout
	BufferLayout layout;
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 3 + 2), 0);
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 3 + 2),sizeof(float) * 3);
	layout.AddLayoutElement(2, GL_FLOAT, false, sizeof(float) * (3 + 3 + 2), sizeof(float) * (3 + 3));
	//Quad
	float quadVertices[] =
	{
		-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
		0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
		0.5f, 0.5f,  0.0f,      0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
		-0.5f, 0.5f,  0.0f,  0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
	};
	unsigned int quadIndices[] =
	{
		0, 1, 2,
		2, 3, 0
	};
	VertexArray quadVao;
	VertexBuffer quadBuffer(quadVertices, sizeof(float) * 8 * 4, layout);
	IndexBuffer quadIndexBuffer(quadIndices, 6);
	quadVao.AddVertexBuffer(&quadBuffer);
	quadVao.SetIndexBuffer(&quadIndexBuffer);
	quadVao.Unbind();
	//Tri
	float triVertices[] =
	{
		-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,        0.0f, 0.0f, 1.0f,    0.5f, 1.0f,
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f
	};
	unsigned int triIndices[] =
	{
		0,1,2
	};
	VertexArray triVao;
	VertexBuffer triBuffer(triVertices, sizeof(float) * 8 * 3, layout);
	IndexBuffer triIndexBuffer(triIndices, 3);
	triVao.AddVertexBuffer(&triBuffer);
	triVao.SetIndexBuffer(&triIndexBuffer);
	triVao.Unbind();
	//Cube
	float cubeVertices[] =
	{
		//Front
		-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,    0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 1.0f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.5f, 0.0f, 1.0f,    0.0f, 1.0f,
		//Back
		-0.5f, -0.5f,  0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.5f,    1.0f, 1.0f,
	};
	unsigned int cubeIndices[] =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
	VertexArray cubeVao;
	VertexBuffer cubeBuffer(cubeVertices, sizeof(float) * 8 * 24, layout);
	IndexBuffer cubeIndexBuffer(cubeIndices, 3 * 2 * 6);
	cubeVao.AddVertexBuffer(&cubeBuffer);
	cubeVao.SetIndexBuffer(&cubeIndexBuffer);
	cubeVao.Unbind();
	//Shader
	auto[cubeVertexSource, cubeFragmentSource] = Shader::ParseShaderFile("shaders/cube.shader");
	Shader cubeShader(cubeVertexSource, cubeFragmentSource);

	auto[vertexSource, fragmentSource] = Shader::ParseShaderFile("shaders/Basic.shader");
	Shader shader(vertexSource, fragmentSource);
	shader.Bind();
	
	Texture texture_0("res/img_cheryl.jpg");
	Texture texture_1("res/img_test.png");

	texture_0.Bind(0);
	texture_1.Bind(1);

	shader.UploadUniformInt("u_Sampler1", 1);

	//TODO: mesh
	Mesh cubeMesh("res/cube.obj");

	//Mesh end
	const float ratio = 1280.0f / 720.f;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), ratio, 0.1f, 1000.0f);
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -10.0f);
	const float speed = .1f;
	float rotateCube = 0.0f;

	Renderer::Init();

	while (!glfwWindowShouldClose(window))
	{		
		shader.Bind();
		shader.UploadUniformFloat("u_Time", glfwGetTime());

		float horizontal = keyMap[GLFW_KEY_D] ? -1 : keyMap[GLFW_KEY_A] ? 1 : 0;
		float vertical = keyMap[GLFW_KEY_S] ? -1 : keyMap[GLFW_KEY_W] ? 1 : 0;
		float depth = keyMap[GLFW_KEY_Z] ? -1 : keyMap[GLFW_KEY_X] ? 1 : 0;
		float rotate = keyMap[GLFW_KEY_Q] ? -1 : keyMap[GLFW_KEY_E] ? 1 : 0;
		rotateCube += rotate;
		cameraPosition += glm::vec3(horizontal, vertical, depth) * speed;
		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		Renderer::Begin(projection * view);

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		glm::mat4 transform = translation * rotation * scale;
		Renderer::Render(&shader, &triVao, transform);
		
		translation = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
		rotation = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		transform = translation * rotation * scale;
		Renderer::Render(&shader, &quadVao, transform);
		
		translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotateCube), glm::vec3(0.0f, 1.0f, 1.0f));
		scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		transform = translation * rotation * scale;
		//Renderer::Render(&shader, &cubeVao, transform);

		cubeMesh.Render(&cubeShader, transform);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}