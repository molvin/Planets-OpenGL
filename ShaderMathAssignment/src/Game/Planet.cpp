#include "Planet.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/Renderer.h"
#include "PlanetSettings.h"
#include "../ImGUI/imgui.h"
#include "filesystem"
#include "../Graphics/Material.h"
#include <sstream>
#include <utility>
#include "../Graphics/ShaderLibrary.h"
#include "../Graphics/Texture.h"


//----- Planet -----
Planet::Planet() : _material(ShaderLibrary::GetShader("planet"))
{
	printf("Created planet");
}

void Planet::Render()
{
	_material.SetUniform("u_ElevationMinMax", glm::vec2(_elevation.Min, _elevation.Max));
	for (std::unique_ptr<PlanetFace>& face : _faces)
	{
		Renderer::Render(&_material, face->GetMesh().GetVertexArray(), _transform.GetMatrix());
	}
}

void Planet::DrawGui(bool& destroy)
{
	std::stringstream ss;
	ss << "Planet " << Id << "Settings";
	ImGui::Begin(ss.str().c_str());

	ImGui::Text("Transform");
	ImGui::InputFloat3("Position", &GetTransform()->Position[0]);
	glm::vec3 euler = GetTransform()->GetEuler();
	const bool changed = ImGui::SliderFloat3("Rotation", &euler[0], -180.0f, 180.0f);
	if (changed) GetTransform()->SetEuler(euler);

	
	ImGui::InputFloat3("Scale", &GetTransform()->Scale[0]);
	ImGui::Text("Settings");
	ImGui::InputInt("Resolution", &_settings.Resolution);
	ImGui::InputFloat("Radius", &_settings.Radius);
	ImGui::InputFloat("Strength", &_settings.Noise[0].Strength);
	ImGui::InputInt("Layer Count", &_settings.Noise[0].LayerCount);
	ImGui::InputFloat("Base Roughness", &_settings.Noise[0].BaseRoughness);
	ImGui::InputFloat("Roughness", &_settings.Noise[0].Roughness);
	ImGui::InputFloat("Persistence", &_settings.Noise[0].Persistence);
	ImGui::InputFloat("MinValue", &_settings.Noise[0].MinValue);
	ImGui::InputFloat3("Center", &_settings.Noise[0].Center.x);



	static ImGradientMark* draggingMark = nullptr;
	static ImGradientMark* selectedMark = nullptr;
	ImGui::GradientEditor(&_gradient, draggingMark, selectedMark);

	const bool regenerate = ImGui::Button("Regenerate");
	destroy = ImGui::Button("Destroy");
	ImGui::End();

	if (regenerate)
		GeneratePlanet();
}
void Planet::GeneratePlanet()
{
	glm::vec3 directions[] =
	{
		glm::vec3(0.0f, 1.0f, 0.0f), //up
		glm::vec3(0.0f, -1.0f, 0.0f), //down
		glm::vec3(1.0f, 0.0f, 0.0f), //right
		glm::vec3(-1.0f, 0.0f, 0.0f), //left
		glm::vec3(0.0f, 0.0f, 1.0f), //forward
		glm::vec3(0.0f, 0.0f, -1.0f), //back
	};
	if(_settings.Noise.empty())
		_settings.Noise.emplace_back();

	_elevation.Reset(1000000, -100000);
	for (int i = 0; i < 6; i++)
	{
		_faces[i] = std::make_unique<PlanetFace>(_settings.Resolution, directions[i], _settings, _elevation);
	}

	printf("Elevation: min: %f, max: %f \n", _elevation.Min, _elevation.Max);

	SetColors();
}
void Planet::SetColors()
{
	const int size = 100 * 4;
	float pixels[size];
	for(int i = 0; i < size; i += 4)
	{
		_gradient.getColorAt((float)i / (float)size, &pixels[i]);
		pixels[i + 3] = 1.0f;
	}

	_texture = new Texture(&pixels[0], 100, 1);
	_material.AddTexture(_texture, 0);
	
}

//----- Planet Face -----
PlanetFace::PlanetFace(const int resolution, const glm::vec3& localUp, const PlanetSettings& settings, MinMaxFloat& elevation)
{
	struct Vertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(1.0f);
	};

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	const int vertexCount = resolution * resolution;
	const int indexCount = (resolution - 1) * (resolution - 1) * 6;
	vertices.resize(vertexCount);
	indices.resize(indexCount);

	const glm::vec3 axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
	const glm::vec3 axisB = glm::cross(localUp, axisA);

	for (int y = 0, vi = 0, ti = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++, vi++)
		{
			const glm::vec2 factor = glm::vec2(x, y) / (float)(resolution - 1);
			glm::vec3 pointOnCube = localUp + (factor.x - 0.5f) * 2.0f * axisA + (factor.y - 0.5f) * 2.0f * axisB;
			glm::vec3 pointOnSphere = glm::normalize(pointOnCube);
			glm::vec3 pointOnPlanet = settings.CalculatePointOnPlanet(pointOnSphere, elevation);

			vertices[vi] = {pointOnPlanet, glm::vec3(1.0f)};
			if (x == resolution - 1 || y == resolution - 1) continue;

			indices[ti] = vi;
			indices[ti + 1] = vi + resolution + 1;
			indices[ti + 2] = vi + resolution;

			indices[ti + 3] = vi;
			indices[ti + 4] = vi + 1;
			indices[ti + 5] = vi + resolution + 1;
			ti += 6;
		}
	}
	for (int i = 0; i < indexCount; i += 3)
	{
		//Triangle
		Vertex* v1 = &vertices[indices[i]];
		Vertex* v2 = &vertices[indices[i + 1]];
		Vertex* v3 = &vertices[indices[i + 2]];

		glm::vec3 normal = glm::normalize(glm::cross(v2->position - v1->position, v3->position - v1->position));
		v1->normal += normal;
		v2->normal += normal;
		v3->normal += normal;
	}
	for (int i = 0; i < vertexCount; i++)
		vertices[i].normal = glm::normalize(vertices[i].normal);

	const int vertexSize = sizeof(float) * 6;
	std::vector<LayoutElement> layout =
	{
		{3, GL_FLOAT, false, vertexSize, 0},
		{3, GL_FLOAT, false, vertexSize, sizeof(float) * 3},
	};
	_mesh = std::make_unique<Mesh>(&vertices[0].position[0], vertexSize, vertexCount, &indices[0], indexCount, layout);
}
