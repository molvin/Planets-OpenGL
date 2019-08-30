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
#include "Input.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

//TODO: Input system
//TODO: folder structure and namespaces
//TODO: ImGUI
//TODO: Window resize event
int main()
{
	//Window
	Window window(1280, 720, "GLFW window");
	//Input
	Input::Init(window.GetWindow());
	glewInit();
	glfwSwapInterval(0);
	//ImGUI init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

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
	Mesh mesh("res/bunny.obj");
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
	const float ratio = window.GetAspectRatio();
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
	const float cameraRotationSpeed = 125.0f;
	const float cameraSpeed = 5.0f;

	glm::vec3 euler = glm::vec3(0.0f);
	while (window.Open())
	{
		
		triMaterial.SetUniformFloat("u_Time", glfwGetTime());
		quadMaterial.SetUniformFloat("u_Time",glfwGetTime() * 3);
		material3D.SetUniformVec3("u_LightDirection", glm::vec3(0.0f, -1.0f, 0.0f));

		if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_2))
		{
			glm::vec2 mouseDir = Input::GetMouseDirection() * cameraRotationSpeed * deltaTime;
			camera.Rotate(mouseDir.x, glm::vec3(0.0f, 1.0f, 0.0f));
			camera.Rotate(mouseDir.y, camera.GetRight());
		}
		float horizontal = Input::GetKey(GLFW_KEY_D) ? -1 : Input::GetKey(GLFW_KEY_A) ? 1 : 0;
		float vertical = Input::GetKey(GLFW_KEY_S) ? -1 : Input::GetKey(GLFW_KEY_W) ? 1 : 0;
		float height = Input::GetKey(GLFW_KEY_Q) ? -1 : Input::GetKey(GLFW_KEY_E) ? 1 : 0;
		camera.Position += camera.GetForward() * vertical * cameraSpeed * deltaTime;
		camera.Position += -camera.GetRight() * horizontal * cameraSpeed * deltaTime;
		camera.Position += glm::vec3(0.0f, 1.0f, 0.0f) * height * cameraSpeed * deltaTime;
				
		Renderer::Begin(projection * camera.GetViewMatrix());
	
		Renderer::Render(&triMaterial, triMesh.GetVertexArray(), triMesh.GetTransform()->GetMatrix());
		Renderer::Render(&material3D, mesh.GetVertexArray(), mesh.GetTransform()->GetMatrix());	
		Renderer::Render(&quadMaterial, quadMesh.GetVertexArray(), quadMesh.GetTransform()->GetMatrix());
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::InputFloat3("Suzanne Position", &mesh.GetTransform()->Position[0]);
		ImGui::SliderFloat3("Suzanne Rotation", &euler[0], 0.0f, 360.0f);
		mesh.GetTransform()->Rotation = glm::quat(euler * 3.1415f / 180.0f);
		ImGui::InputFloat3("Suzanne Scale", &mesh.GetTransform()->Scale[0]);

		ImGui::End();

		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();

		float currentTime = glfwGetTime();
		deltaTime = currentTime - time;
		time = currentTime;

		Input::Update();
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}