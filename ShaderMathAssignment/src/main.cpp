#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Texture.h"
#include "Mesh.h"
#include "Window.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Input.h"
#include "ImGUI/imgui.h"
#include "FrameBuffer.h"
#include "ImGuiRenderer.h"

//TODO: folder structure and namespaces
//TODO: Assimp
int main()
{
	//Window
	Window window(1280, 720, "GLFW window");

	//Input
	Input::Init(window.GetWindow());
	glewInit();

	ImGuiRenderer::Init(window.GetWindow());

	//Meshes
	Mesh mesh("res/TRex.fbx");
	Mesh suzanne("res/suzanne.obj");
	suzanne.GetTransform()->Position = glm::vec3(0.0f, 0.0f, 5.0f);
	
	//Shaders
	auto[cubeVertexSource, cubeFragmentSource] = Shader::ParseShaderFile("shaders/cube.shader");
	Shader shader3D(cubeVertexSource, cubeFragmentSource);
	auto[toonVertSource, toonFragSource] = Shader::ParseShaderFile("shaders/toon.shader");
	Shader toonShader(toonVertSource, toonFragSource);

	//Materials
	Material material3D(&shader3D);
	Material toonMaterial(&toonShader);
	suzanne.SetMaterial(&toonMaterial);
	mesh.SetMaterial(&material3D);
	
	//Textures
	Texture texture_0("res/uv_test.jpg", 0);
	Texture texture_1("res/img_cheryl.jpg", 1);
	Texture toonTexture("res/toon.png", 0);
	material3D.SetUniform("u_Sampler1", 1);
	material3D.AddTexture(&texture_0);
	material3D.AddTexture(&texture_1);
	toonMaterial.AddTexture(&toonTexture);

	//Matrices
	const float ratio = window.GetAspectRatio();
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), ratio, 0.1f, 1000.0f);

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

	FrameBuffer frameBuffer(2000, 2000);

	//Main loop
	Renderer::Init();
	while (window.Open())
	{
		if(window.SizeChanged())	//TODO: Figure out a better way to deal with this
		{
			projection = glm::perspective(glm::radians(60.0f), window.GetAspectRatio(), 0.1f, 1000.0f);
		}

		//Camera movement
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
		
		material3D.SetUniform("u_EyePosition", camera.Position);
		//Rendering
		frameBuffer.Bind();
		glViewport(0, 0, 2000, 2000);
		Renderer::Begin(projection * camera.GetViewMatrix());
		Renderer::Render(&material3D, mesh.GetVertexArray(), mesh.GetTransform()->GetMatrix());
		Renderer::Render(&toonMaterial, suzanne.GetVertexArray(), mesh.GetTransform()->GetMatrix() * suzanne.GetTransform()->GetMatrix());
		frameBuffer.Unbind();
		glViewport(0, 0, window.GetWidth(), window.GetHeight()); 
		Renderer::Begin(projection * camera.GetViewMatrix());
		Renderer::Render(&suzanne);
		Renderer::Render(&mesh);
		
		//ImGUI
		ImGuiRenderer::Begin();

		mesh.DrawGui("Mesh");
		suzanne.DrawGui("Suzanne");

		//Extra settings
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

		ImGuiRenderer::End();

		//End of loop
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();

		float currentTime = glfwGetTime();
		deltaTime = currentTime - time;
		time = currentTime;

		Input::Update();
	}
	ImGuiRenderer::Terminate();
	glfwTerminate();
	return 0;
}