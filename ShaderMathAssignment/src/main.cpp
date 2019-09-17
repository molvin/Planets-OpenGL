#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Graphics/Shader.h"
#include "Graphics/Renderer.h"
#include "glm/glm.hpp"
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"
#include "Application/Window.h"
#include "Graphics/Material.h"
#include "Core/Transform.h"
#include "Application/Input.h"
#include "Graphics/FrameBuffer.h"
#include "ImGUI/ImGuiRenderer.h"
#include "Game/Planet.h"
#include "Game/PlanetSettings.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/CubeMap.h"
#include "Graphics/Light/DirectionalLight.h"
#include "Graphics/Light/PointLight.h"
#include "Game/Camera.h"
#include "Core/Time.h"
#include "Renderable.h"

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

//TODO: Cleanup
//TODO: better planet generation, multiple noise filters
//TODO: mesh shouldn't have material or transform, make some other structure if you wanna hold that data

int main()
{
	//Init
	Window window(1280, 720, "GLFW window");
	Input::Init(window.GetWindow());
	glewInit();
	ImGuiRenderer::Init(window.GetWindow());
	Time::Init();

	std::vector<LayoutElement> layout = 
	{
		{3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), 0},
		{2, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * (3)},
		{3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * (3 + 2)}
	};
	//Meshes
	Mesh mesh("res/TRex.fbx");
	Mesh cube(cubeVertexData, sizeof(float) * 8, 24, cubeIndexData, 36, layout);
	Transform meshTransform, cubeTransform;
	meshTransform.Position = glm::vec3(0.0f, 0.9f, 0.0f);
	meshTransform.Scale = glm::vec3(0.1f);
	cubeTransform.Scale = glm::vec3(10.f, 1.f, 10.f);
	cubeTransform.Position = glm::vec3(0.0f, -5, 0.0f);

	//Shaders
	Shader shader3D("shaders/cube.shader");
	Shader postProcessShader("shaders/post_process.shader");
	Shader planetShader("shaders/planet.shader");
	Shader skyboxShader("shaders/skybox.shader");

	//Materials
	Material material3D(&shader3D);
	Material planetMaterial(&planetShader);
	Material postProcessMaterial(&postProcessShader);
	Material skyboxMaterial(&skyboxShader);
	postProcessMaterial.SetUniform("u_FrameDepth", 1);

	//Frame Buffers
	FrameBuffer lightBuffer(4096, 4096);
	FrameBuffer frameBuffer(2000, 2000);

	//Textures
	Texture uvTestTexure("res/uv_test.jpg");
	Texture toonTexture("res/toon.png");
	Texture planetTexture("res/gradient.png");
	CubeMap  skybox(skyBoxImages);
	material3D.SetUniform("u_LightBuffer", 1);
	planetMaterial.SetUniform("u_Sampler", 0);
	postProcessMaterial.AddTexture(frameBuffer.GetColorTexture(), 0);
	postProcessMaterial.AddTexture(frameBuffer.GetDepthTexture(), 1);
	material3D.AddTexture(&uvTestTexure, 0);
	material3D.AddTexture(lightBuffer.GetDepthTexture(), 1);
	planetMaterial.AddTexture(&planetTexture, 0);
	
	//Camera
	Camera camera(glm::vec3(19.0f, 9.4f, -9.74f));
	camera.Direction = glm::vec3(-0.859f, -0.329f, 0.392f);

	//Light
	float specularIntensity = 0.4f;
	DirectionalLight directionalLight;
	PointLight pointLight;
	pointLight.Position = glm::vec3(0.0f, 3.0f, 0.0f);
	pointLight.Color = glm::vec3(0.7f, 0.3f, 0.4f);
	pointLight.Radius = 15.0f;

	Planet planet("res/Planet.txt");

	Renderable renderable(&cube, &material3D);
	renderable.GetTransform()->Position = glm::vec3(0.534f, 1.35f, 304.0f);
	renderable.GetTransform()->Rotate(56.f, glm::vec3(0.0f, 1.0f, 0.0f));
	printf("%s", renderable.GetSaveFormat().c_str());
	
	//Main loop
	Renderer::Init();
	while (window.Open())
	{
		//Uniform setting //TODO: some of these are set by reference, which might cause unexpected behaviour if one goes out of scope
		planetMaterial.SetUniform("u_LightDirection", directionalLight.Direction);
		planetMaterial.SetUniform("u_DiffuseColor", directionalLight.Color);
		planetMaterial.SetUniform("u_DiffuseIntensity", directionalLight.Intensity);
		material3D.SetUniform("u_SpecularIntensity", specularIntensity);
		planetMaterial.SetUniform("u_SpecularIntensity", specularIntensity);

		material3D.SetUniform("u_EyePosition", camera.Position);
		material3D.SetUniform("u_LightViewProjection", directionalLight.GetLightProjection() * directionalLight.GetLightView());
		
		planetMaterial.SetUniform("u_EyePosition", camera.Position);
		skyboxMaterial.SetUniform("u_Projection", window.GetProjectionMatrix());
		skyboxMaterial.SetUniform("u_View", camera.GetViewMatrix());
	
		material3D.SetUniform("u_PointLightCount", 1);
		directionalLight.UploadToMaterial("u_DirectionalLight", material3D);
		directionalLight.UploadToMaterial("u_DirectionalLight", planetMaterial);
		pointLight.UploadToMaterial("u_PointLights", 0, material3D);
		//Rendering
		//Light render for shadows
		lightBuffer.Bind();
		{
			glViewport(0, 0, 4096, 4096);
			Renderer::Begin(directionalLight.GetLightProjection() * directionalLight.GetLightView());
			Renderer::Render(&material3D, mesh.GetVertexArray(), meshTransform.GetMatrix());
			Renderer::Render(&material3D, cube.GetVertexArray(), cubeTransform.GetMatrix());
			planet.Render(planetMaterial);
		}
		lightBuffer.Unbind();

		//Render to frame buffer
		frameBuffer.Bind();
		{
			glViewport(0, 0, 2048, 2048);
			Renderer::Begin(window.GetProjectionMatrix() * camera.GetViewMatrix());
			Renderer::Render(&material3D, mesh.GetVertexArray(), meshTransform.GetMatrix());
			Renderer::Render(&material3D, cube.GetVertexArray(), cubeTransform.GetMatrix());
			planet.Render(planetMaterial);
			skybox.Bind(0);
			Transform transform;
			Renderer::Render(&skyboxMaterial, cube.GetVertexArray(), transform.GetMatrix());
		}
		frameBuffer.Unbind();
		
		//Render from frame buffer
		glViewport(0, 0, window.GetWidth(), window.GetHeight());
		Renderer::Begin(window.GetProjectionMatrix() * camera.GetViewMatrix());
		Renderer::RenderFrameBuffer(postProcessMaterial);

		//ImGUI
		ImGuiRenderer::Begin();
		{
			//Planet
			planet.RenderGui();
			//Meshes
			//TODO
			//Light settings
			directionalLight.DrawGui();
			//Camera settings
			camera.DrawGui();
		}
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
