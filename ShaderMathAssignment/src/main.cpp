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

//TODO: folder structure and namespaces
//TODO: Assimp
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
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), 0);
	layout.AddLayoutElement(2, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * 3);
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * (3 + 2));
	//Shapes
	//Quad
	float quadVertices[] =
	{
		-0.5f, -0.5f,  0.0f,    0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.0f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.0f,    1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.0f,    0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
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
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f,     0.5f, 1.0f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,    1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	};
	unsigned int triIndices[] =
	{
		0,1,2
	};
	Mesh triMesh(triVertices, sizeof(float) * 8, 3, triIndices, 3, layout);
	triMesh.GetTransform()->Position = glm::vec3(-3.0f, 0.0f, 0.0f);
	//Suzanne
	Mesh mesh("res/TRex.fbx");
	Mesh suzanne("res/suzanne.obj");
	suzanne.GetTransform()->Position = glm::vec3(0.0f, 0.0f, 5.0f);
	//Shader
	auto[cubeVertexSource, cubeFragmentSource] = Shader::ParseShaderFile("shaders/cube.shader");
	Shader shader3D(cubeVertexSource, cubeFragmentSource);
	//auto[vertexSource, fragmentSource] = Shader::ParseShaderFile("shaders/Basic.shader");
	//Shader shader2D(vertexSource, fragmentSource);
	auto[toonVertSource, toonFragSource] = Shader::ParseShaderFile("shaders/toon.shader");
	Shader toonShader(toonVertSource, toonFragSource);
	//Materials
	//Material triMaterial(&shader2D);
	//Material quadMaterial(&shader2D);
	Material material3D(&shader3D);
	Material toonMaterial(&toonShader);
	//Textures

	Texture texture_0("res/uv_test.jpg");
	Texture texture_1("res/img_cheryl.jpg");
	texture_0.Bind(1);
	texture_1.Bind(2);
	material3D.SetUniform("u_Sampler0",1);
	material3D.SetUniform("u_Sampler1", 2);
	Texture toonTexture("res/toon.png");
	toonTexture.Bind(3);
	toonMaterial.SetUniform("u_Sampler", 3);

	
	//Matrices
	const float ratio = window.GetAspectRatio();
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), ratio, 0.1f, 1000.0f);
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

	glm::vec3 lightDir = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 toonColor = glm::vec3(0.0f, 1.0f, 0.0f);
	float specularIntensity = 0.4f;
	while (window.Open())
	{
		if(window.SizeChanged())
		{
			printf("Window size changed\n");
			projection = glm::perspective(glm::radians(60.0f), window.GetAspectRatio(), 0.1f, 1000.0f);
		}
		
		//triMaterial.SetUniform("u_Time", (float)glfwGetTime());
		//quadMaterial.SetUniform("u_Time",(float)glfwGetTime() * 3);
		material3D.SetUniform("u_EyePosition", camera.Position);
		
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
		Renderer::Render(&material3D, triMesh.GetVertexArray(), triMesh.GetTransform()->GetMatrix());
		Renderer::Render(&material3D, mesh.GetVertexArray(), mesh.GetTransform()->GetMatrix());
		Renderer::Render(&material3D, quadMesh.GetVertexArray(), quadMesh.GetTransform()->GetMatrix());
		Renderer::Render(&toonMaterial, suzanne.GetVertexArray(), mesh.GetTransform()->GetMatrix() * suzanne.GetTransform()->GetMatrix());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		mesh.DrawGui("Mesh");
		suzanne.DrawGui("Suzanne");
		quadMesh.DrawGui("Quad");
		triMesh.DrawGui("Tri");

		ImGui::Begin("Light Direction");
		ImGui::SliderFloat3("Direction", &lightDir[0], -1.0f, 1.0f);
		ImGui::InputFloat("Specular Intensity", &specularIntensity);
		ImGui::ColorPicker3("Toon Color", &toonColor[0]);
		lightDir = normalize(lightDir);
		material3D.SetUniform("u_LightDirection", lightDir);
		toonMaterial.SetUniform("u_LightDirection", lightDir);
		toonMaterial.SetUniform("u_Color", toonColor);
		material3D.SetUniform("u_SpecularIntensity", specularIntensity);
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