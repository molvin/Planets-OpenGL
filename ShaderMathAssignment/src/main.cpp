#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Graphics/Shader.h"
#include "Graphics/Renderer.h"
#include "glm/glm.hpp"
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"
#include "Application/Window.h"
#include "Graphics/Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Application/Input.h"
#include "ImGUI/imgui.h"
#include "Graphics/FrameBuffer.h"
#include "ImGUI/ImGuiRenderer.h"
#include "Time.h"

int main()
{
	//Init
	Window window(1280, 720, "GLFW window");
	Input::Init(window.GetWindow());
	glewInit();
	ImGuiRenderer::Init(window.GetWindow());
	Time::Init();

	//Meshes
	Mesh mesh("res/TRex.fbx");
	Mesh suzanne("res/suzanne.obj");
	suzanne.GetTransform()->Position = glm::vec3(0.0f, 0.0f, 5.0f);
	
	//Shaders
	auto[cubeVertexSource, cubeFragmentSource] = Shader::ParseShaderFile("shaders/cube.shader");
	Shader shader3D(cubeVertexSource, cubeFragmentSource);
	auto[toonVertSource, toonFragSource] = Shader::ParseShaderFile("shaders/toon.shader");
	Shader toonShader(toonVertSource, toonFragSource);
	auto[postProcessVertSource, postProcessFragSource] = Shader::ParseShaderFile("shaders/post_process.shader");
	Shader postProcessShader(postProcessVertSource, postProcessFragSource);
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

	//Camera
	Camera camera(glm::vec3(0.0f, 0.0f, -10.0f));

	//Misc
	glm::vec3 lightDir = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 toonColor = glm::vec3(0.0f, 1.0f, 0.0f);
	float specularIntensity = 0.4f;
	FrameBuffer frameBuffer(2000, 2000);

	//Main loop
	Renderer::Init();
	while (window.Open())
	{	
		//Camera movement
		camera.Update();

		material3D.SetUniform("u_EyePosition", camera.Position);
		//Rendering
		frameBuffer.Bind();
		glViewport(0, 0, 2000, 2000);
		Renderer::Begin(window.GetProjectionMatrix() * camera.GetViewMatrix());
		Renderer::Render(&material3D, mesh.GetVertexArray(), mesh.GetTransform()->GetMatrix());
		Renderer::Render(&toonMaterial, suzanne.GetVertexArray(), suzanne.GetTransform()->GetMatrix());
		frameBuffer.Unbind();
		glViewport(0, 0, window.GetWidth(), window.GetHeight());

		Renderer::Begin(window.GetProjectionMatrix() * camera.GetViewMatrix());
		frameBuffer.BindTexture();
		postProcessShader.Bind();
		Renderer::RenderFrameBuffer();
		
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

		Input::Update();
		Time::Tick();
	}
	ImGuiRenderer::Terminate();
	glfwTerminate();
	return 0;
}