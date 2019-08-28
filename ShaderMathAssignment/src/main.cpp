#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Texture.h"
#include <map>
#include "Mesh.h"
#include "Window.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "../Input.h"

//TODO: Input system
//TODO: folder structure and namespaces

int keyMap[GLFW_KEY_LAST];

void OnKeyEvent(GLFWwindow* window, const int key, const int scanCode, const int action, const int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);

	if (action == GLFW_PRESS) keyMap[key] = true;
	else if (action == GLFW_RELEASE) keyMap[key] = false;
}	
int main()
{
	//Window
	const int screenWidth = 1280;
	const int screenHeight = 720;
	Window window(screenWidth, screenHeight, "GLFW window");
	//Input
	//glfwSetKeyCallback(window.GetWindow(), OnKeyEvent);
	Input::Init(window.GetWindow());
	glewInit();
	glfwSwapInterval(0);
	//Buffer layout
	BufferLayout layout;
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 3 + 2), 0);
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 3 + 2),sizeof(float) * 3);
	layout.AddLayoutElement(2, GL_FLOAT, false, sizeof(float) * (3 + 3 + 2), sizeof(float) * (3 + 3));
	//Shapes
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
	Mesh quadMesh(quadVertices, sizeof(float) * 8, 4, quadIndices, 6, layout);
	quadMesh.GetTransform()->Position = glm::vec3(3.0f, 0.0f, 0.0f);
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
	Mesh triMesh(triVertices, sizeof(float) * 8, 3, triIndices, 3, layout);
	triMesh.GetTransform()->Position = glm::vec3(-3.0f, 0.0f, 0.0f);
	//Suzanne
	Mesh mesh("res/suzanne.obj");
	//Shader
	auto[cubeVertexSource, cubeFragmentSource] = Shader::ParseShaderFile("shaders/cube.shader");
	Shader cubeShader(cubeVertexSource, cubeFragmentSource);
	auto[vertexSource, fragmentSource] = Shader::ParseShaderFile("shaders/Basic.shader");
	Shader shader(vertexSource, fragmentSource);
	shader.Bind();	
	//Textures
	Texture texture_0("res/uv_test.jpg");
	Texture texture_1("res/img_cheryl.jpg");
	texture_0.Bind(0);
	texture_1.Bind(1);
	shader.UploadUniformInt("u_Sampler1", 1);
	//Matrices
	const float ratio = 1280.0f / 720.f;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), ratio, 0.1f, 1000.0f);
	//Materials
	Material triMaterial(&shader);
	Material quadMaterial(&shader);
	Material material3D(&cubeShader);
	//Main loop
	Renderer::Init();
	//Camera
	Camera camera;
	camera.Position = glm::vec3(0.0f, 0.0f, -10.0f);
	//Misc
	float time = glfwGetTime();
	float deltaTime = 0.016f;
	while (window.Open())
	{
		triMaterial.SetUniformFloat("u_Time", glfwGetTime());
		quadMaterial.SetUniformFloat("u_Time",glfwGetTime() * 3);

		float horizontal = Input::GetKey(GLFW_KEY_D) ? -1 : Input::GetKey(GLFW_KEY_A) ? 1 : 0;
		float vertical = Input::GetKey(GLFW_KEY_S) ? -1 : Input::GetKey(GLFW_KEY_W) ? 1 : 0;
		float depth = Input::GetKey(GLFW_KEY_Z) ? -1 : Input::GetKey(GLFW_KEY_X) ? 1 : 0;
		camera.Position += glm::vec3(horizontal, vertical, depth) * 3.0f * deltaTime;

		Renderer::Begin(projection * camera.GetViewMatrix());
	
		Renderer::Render(&triMaterial, triMesh.GetVertexArray(), triMesh.GetTransform()->GetMatrix());
		mesh.GetTransform()->Rotate(deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		Renderer::Render(&material3D, mesh.GetVertexArray(), mesh.GetTransform()->GetMatrix());	
		Renderer::Render(&quadMaterial, quadMesh.GetVertexArray(), quadMesh.GetTransform()->GetMatrix());
	
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();

		float currentTime = glfwGetTime();
		deltaTime = currentTime - time;
		time = currentTime;

		Input::Update();
	}
	glfwTerminate();
	return 0;
}