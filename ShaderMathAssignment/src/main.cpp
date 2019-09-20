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
#include "Graphics/VertexBuffer.h"
#include "Graphics/CubeMap.h"
#include "Graphics/Light/DirectionalLight.h"
#include "Graphics/Light/PointLight.h"
#include "Game/Camera.h"
#include "Core/Time.h"
#include "Game/SolarSystem.h"
#include "Graphics/ShaderLibrary.h"

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
	Shader postProcessShader("shaders/post_process.shader");
	Shader skyboxShader("shaders/skybox.shader");

	{
		auto material3D = std::make_shared<Shader>("shaders/cube.shader");
		ShaderLibrary::AddShader("shader3D", material3D);
		auto planetShader = std::make_shared<Shader>("shaders/planet.shader");
		ShaderLibrary::AddShader("planet", planetShader);
	}
	Shader* shader3D = ShaderLibrary::GetShader("shader3D");

	//Materials
	Material material3D(shader3D);
	Material postProcessMaterial(&postProcessShader);
	Material skyboxMaterial(&skyboxShader);
	postProcessMaterial.SetUniform("u_FrameDepth", 1);

	//Frame Buffers
	FrameBuffer lightBuffer(4096, 4096);
	FrameBuffer frameBuffer(2000, 2000);

	//Textures
	Texture uvTestTexure("res/uv_test.jpg");
	CubeMap  skybox(skyBoxImages);
	material3D.SetUniform("u_LightBuffer", 1);
	postProcessMaterial.AddTexture(frameBuffer.GetColorTexture(), 0);
	postProcessMaterial.AddTexture(frameBuffer.GetDepthTexture(), 1);
	material3D.AddTexture(&uvTestTexure, 0);
	material3D.AddTexture(lightBuffer.GetDepthTexture(), 1);
	
	//Camera
	Camera camera(glm::vec3(19.0f, 9.4f, -9.74f));
	camera.Direction = glm::vec3(-0.859f, -0.329f, 0.392f);

	//Light
	float specularIntensity = 0.4f;
	PointLight pointLight;
	pointLight.Position = glm::vec3(0.0f, 3.0f, 0.0f);
	pointLight.Color = glm::vec3(0.7f, 0.3f, 0.4f);
	pointLight.Radius = 15.0f;

	SolarSystem solarSystem;
	solarSystem.SetShadowBuffer(lightBuffer.GetDepthTexture());

	
	//Main loop
	Renderer::Init();
	while (window.Open())
	{
		DirectionalLight& directionalLight = solarSystem.GetGodLight();
		//Uniform setting
		material3D.SetUniform("u_SpecularIntensity", specularIntensity);

		material3D.SetUniform("u_EyePosition", camera.Position);
		material3D.SetUniform("u_LightViewProjection", directionalLight.GetLightProjection() * directionalLight.GetLightView());
		
		skyboxMaterial.SetUniform("u_Projection", window.GetProjectionMatrix());
		skyboxMaterial.SetUniform("u_View", camera.GetViewMatrix());
	
		material3D.SetUniform("u_PointLightCount", 1);
		directionalLight.UploadToMaterial("u_DirectionalLight", material3D);
		pointLight.UploadToMaterial("u_PointLights", 0, material3D);


		solarSystem.Update(camera, window);
		
		//Rendering
		//Light render for shadows
		lightBuffer.Bind();
		{
			glViewport(0, 0, 4096, 4096);
			Renderer::Begin(directionalLight.GetLightProjection() * directionalLight.GetLightView());
			Renderer::Render(&material3D, mesh.GetVertexArray(), meshTransform.GetMatrix());
			Renderer::Render(&material3D, cube.GetVertexArray(), cubeTransform.GetMatrix());
			solarSystem.Render(camera.Position);
		}
		lightBuffer.Unbind();

		//Render to frame buffer
		frameBuffer.Bind();
		{
			glViewport(0, 0, 2048, 2048);
			Renderer::Begin(window.GetProjectionMatrix() * camera.GetViewMatrix());
			Renderer::Render(&material3D, mesh.GetVertexArray(), meshTransform.GetMatrix());
			Renderer::Render(&material3D, cube.GetVertexArray(), cubeTransform.GetMatrix());
			solarSystem.Render(camera.Position);
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
			solarSystem.DrawGui();
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
