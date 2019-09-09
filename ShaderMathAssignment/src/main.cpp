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
#include "Game/Planet.h"
#include "Game/PlanetSettings.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/CubeMap.h"

float cubeVertexData[] = {
	//Front face
	-1.f, -1.f, +1.f,	0.f, 0.f,		0.f, 0.f, 1.f,  // 0
	+1.f, -1.f, +1.f,	1.f, 0.f,		0.f, 0.f, 1.f,  // 1
	+1.f, +1.f, +1.f,	1.f, 1.f,		0.f, 0.f, 1.f,  // 2
	-1.f, +1.f, +1.f,	0.f, 1.f,		0.f, 0.f, 1.f, // 3

	//back face
	-1.f, -1.f, -1.f,	0.f, 0.f,		0.f, 0.f, -1.f,  // 4
	+1.f, -1.f, -1.f,	1.f, 0.f,		0.f, 0.f, -1.f,  // 5
	+1.f, +1.f, -1.f,	1.f, 1.f,		0.f, 0.f, -1.f,  // 6
	-1.f, +1.f, -1.f,	0.f, 1.f,		0.f, 0.f, -1.f,  // 7

	// right face
	+1.f, -1.f, +1.f,	0.f, 0.f,		1.f, 0.f, 0.f,  // 8
	+1.f, -1.f, -1.f,	1.f, 0.f,		1.f, 0.f, 0.f,  // 9
	+1.f, +1.f, -1.f,	1.f, 1.f,		1.f, 0.f, 0.f,  // 10
	+1.f, +1.f, +1.f,	0.f, 1.f,		1.f, 0.f, 0.f,  // 11

	// left face
	-1.f, -1.f, +1.f,	0.f, 0.f,		-1.f, 0.f, 0.f,  // 12
	-1.f, -1.f, -1.f,	1.f, 0.f,		-1.f, 0.f, 0.f,  // 13
	-1.f, +1.f, -1.f,	1.f, 1.f,		-1.f, 0.f, 0.f,  // 14
	-1.f, +1.f, +1.f,	 0.f, 1.f,		-1.f, 0.f, 0.f, // 15

	// top face
	-1.f, +1.f, +1.f,	0.f, 0.f,		0.f, 1.f, 0.f,  // 16
	+1.f, +1.f, +1.f,	1.f, 0.f,		0.f, 1.f, 0.f,  // 17
	+1.f, +1.f, -1.f,	1.f, 1.f,		0.f, 1.f, 0.f,  // 18
	-1.f, +1.f, -1.f,	0.f, 1.f,		0.f, 1.f, 0.f, // 19

	// bottom face
	-1.f, -1.f, +1.f,	0.f, 0.f,		0.f, -1.f, 0.f,  // 20
	+1.f, -1.f, +1.f,	1.f, 0.f,		0.f, -1.f, 0.f,  // 21
	+1.f, -1.f, -1.f,	1.f, 1.f,		0.f, -1.f, 0.f,  // 22
	-1.f, -1.f, -1.f,	0.f, 1.f,		0.f, -1.f, 0.f,  // 23

};
unsigned int cubeIndexData[] = {
	// front
	0, 1, 2,        0, 2, 3,
	// back
	4, 5, 6,        4, 6, 7,
	// right
	8, 9, 10,        8, 10, 11,
	// left
	12, 13, 14,        12, 14, 15,
	// top
	16, 17, 18,        16, 18, 19,
	//bottom
	20, 21, 22,        20, 22, 23,
};

const std::string skyBoxImages[] =
{
	"res/Skybox/purplenebula_rt.tga",
	"res/Skybox/purplenebula_lf.tga",
	"res/Skybox/purplenebula_up.tga",
	"res/Skybox/purplenebula_dn.tga",
	"res/Skybox/purplenebula_ft.tga",
	"res/Skybox/purplenebula_bk.tga",
};

//TODO: texture slots should be managed by materials not textures
//TODO: Set light color (ambient, diffuse, specular), make directional light class
//TODO: Cleanup
//TODO: better planet generation, multiple noise filters
//TODO: planet save settings
//TODO: fråga emil om att gömma seams

int main()
{
	//Init
	Window window(1280, 720, "GLFW window");
	Input::Init(window.GetWindow());
	glewInit();
	ImGuiRenderer::Init(window.GetWindow());
	Time::Init();

	BufferLayout layout;
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), 0);
	layout.AddLayoutElement(2, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * (3));
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * (3 + 2));
	
	//Meshes
	Mesh mesh("res/TRex.fbx");
	Mesh cube(cubeVertexData, sizeof(float) * 8, 24, cubeIndexData, 36, layout);
	mesh.GetTransform()->Position = glm::vec3(-5.0f, -5.0f, 0.0f);
	cube.GetTransform()->Scale = glm::vec3(10);
	cube.GetTransform()->Position = glm::vec3(0.0f, -15, 0.0f);

	//Shaders
	auto[cubeVertexSource, cubeFragmentSource] = Shader::ParseShaderFile("shaders/cube.shader");
	Shader shader3D(cubeVertexSource, cubeFragmentSource);
	auto[postProcessVertSource, postProcessFragSource] = Shader::ParseShaderFile("shaders/post_process.shader");
	Shader postProcessShader(postProcessVertSource, postProcessFragSource);
	auto[planetVertSource, planetFragSource] = Shader::ParseShaderFile("shaders/planet.shader");
	Shader planetShader(planetVertSource, planetFragSource);
	auto[skyboxVertSource, skyboxFragSource] = Shader::ParseShaderFile("shaders/skybox.shader");
	Shader skyboxShader(skyboxVertSource, skyboxFragSource);

	//Materials
	Material material3D(&shader3D);
	Material planetMaterial(&planetShader);
	Material postProcessMaterial(&postProcessShader);
	Material skyboxMaterial(&skyboxShader);
	mesh.SetMaterial(&material3D);
	postProcessMaterial.SetUniform("u_FrameDepth", 1);

	FrameBuffer lightBuffer(4096, 4096);
	FrameBuffer frameBuffer(2000, 2000);

	//Textures
	Texture uvTestTexure("res/uv_test.jpg", 0);
	Texture toonTexture("res/toon.png", 0);
	Texture planetTexture("res/gradient.png", 0);
	CubeMap  skybox(skyBoxImages);
	material3D.SetUniform("u_LightBuffer", 1);
	planetMaterial.SetUniform("u_Sampler", 0);
	postProcessMaterial.AddTexture(frameBuffer.GetColorTexture());
	postProcessMaterial.AddTexture(frameBuffer.GetDepthTexture());
	material3D.AddTexture(&uvTestTexure);
	material3D.AddTexture(lightBuffer.GetDepthTexture());
	planetMaterial.AddTexture(&planetTexture);
	
	//Camera
	Camera camera(glm::vec3(19.0f, 9.4f, -9.74f));
	camera.Direction = glm::vec3(-0.859f, -0.329f, 0.392f);

	//Light
	glm::vec3 lightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
	float specularIntensity = 0.4f;
	glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -30.f, 30.0f);
	glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f), lightDir, glm::vec3(0.0f, 1.0f, 0.0f));

	//Planet
	PlanetSettings settings;
	settings.Noise.push_back(NoiseSettings());
	settings.Resolution = 3;
	Planet planet(settings);

	//Main loop
	Renderer::Init();
	while (window.Open())
	{
		skyboxMaterial.SetUniform("u_Projection", window.GetProjectionMatrix());
		skyboxMaterial.SetUniform("u_View", camera.GetViewMatrix());
		
		material3D.SetUniform("u_EyePosition", camera.Position);
		planetMaterial.SetUniform("u_EyePosition", camera.Position);

		lightView = glm::lookAt(glm::vec3(0.0f), lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
		material3D.SetUniform("u_LightViewProjection", lightProjection * lightView);

		//TODO: this could be done better
		material3D.SetUniform("u_LightDirection", lightDir);
		planetMaterial.SetUniform("u_LightDirection", lightDir);
		material3D.SetUniform("u_SpecularIntensity", specularIntensity);
		planetMaterial.SetUniform("u_SpecularIntensity", specularIntensity);

		//Rendering
		lightBuffer.Bind();

		//Light render
		glViewport(0, 0, 4096, 4096);
		Renderer::Begin(lightProjection * lightView);
		Renderer::Render(&material3D, mesh.GetVertexArray(), mesh.GetTransform()->GetMatrix());
		Renderer::Render(&material3D, cube.GetVertexArray(), cube.GetTransform()->GetMatrix());
		lightBuffer.Unbind();

		//Render to frame buffer
		frameBuffer.Bind();
		glViewport(0, 0, 2000, 2000);
		Renderer::Begin(window.GetProjectionMatrix() * camera.GetViewMatrix());		
		Renderer::Render(&material3D, mesh.GetVertexArray(), mesh.GetTransform()->GetMatrix());
		Renderer::Render(&material3D, cube.GetVertexArray(), cube.GetTransform()->GetMatrix());
		planet.Render(planetMaterial);
		skybox.Bind(0);
		Transform transform;
		Renderer::Render(&skyboxMaterial, cube.GetVertexArray(), transform.GetMatrix());
		frameBuffer.Unbind();
		
		//Render from frame buffer
		glViewport(0, 0, window.GetWidth(), window.GetHeight());
		Renderer::Begin(window.GetProjectionMatrix() * camera.GetViewMatrix());
		Renderer::RenderFrameBuffer(postProcessMaterial);

		//ImGUI
		ImGuiRenderer::Begin();

		planet.RenderGui();	
		mesh.DrawGui("Mesh");
		cube.DrawGui("Cube");

		//Light settings
		ImGui::Begin("Light Direction");
		ImGui::SliderFloat3("Direction", &lightDir[0], -1.0f, 1.0f);
		ImGui::InputFloat("Specular Intensity", &specularIntensity);
		lightDir = normalize(lightDir);
		ImGui::End();

		//Camera settings //TODO: add fov, clip planes
		ImGui::Begin("Camera");
		ImGui::InputFloat3("Position", &camera.Position[0]);
		ImGui::SliderFloat3("Direction", &camera.Direction[0], -1.0f, 1.0f);
		camera.Direction = glm::normalize(camera.Direction);
		ImGui::End();

		ImGuiRenderer::End();

		//Camera movement
		camera.Update();

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
