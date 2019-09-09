#include "Planet.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/Renderer.h"
#include "PlanetSettings.h"
#include "../ImGUI/imgui.h"


//----- Planet -----
Planet::Planet(PlanetSettings& settings)
{
	_settings = &settings;
	GeneratePlanet();
}
Planet::~Planet()
{
	//delete[] _faces;
}

void Planet::Render(Material& material)
{
	material.SetUniform("u_ElevationMinMax", glm::vec2(_elevation.Min, _elevation.Max));
	for (auto& face : _faces)
	{
		Renderer::Render(&material, face->GetMesh().GetVertexArray(), _transform.GetMatrix());
	}
}

void Planet::RenderGui()
{
	ImGui::Begin("Planet Settings");

	ImGui::InputInt("Resolution", &_settings->Resolution);
	ImGui::InputFloat("Radius", &_settings->Radius);
	ImGui::InputFloat("Strength", &_settings->Noise[0].Strength);
	ImGui::InputInt("Layer Count", &_settings->Noise[0].LayerCount);
	ImGui::InputFloat("Base Roughness", &_settings->Noise[0].BaseRoughness);
	ImGui::InputFloat("Roughness", &_settings->Noise[0].Roughness);
	ImGui::InputFloat("Persistence", &_settings->Noise[0].Persistence);
	ImGui::InputFloat("MinValue", &_settings->Noise[0].MinValue);
	ImGui::InputFloat3("Center", &_settings->Noise[0].Center.x);


	const bool regenerate = ImGui::Button("Regenerate");
	ImGui::End();

	if (regenerate)
		GeneratePlanet();
}

void Planet::GeneratePlanet()
{
	glm::vec3 directions[] =
	{
		glm::vec3(0.0f,  1.0f,  0.0f),	 //up
		glm::vec3(0.0f, -1.0f,  0.0f), //down
		glm::vec3(1.0f,  0.0f,  0.0f), //right
		glm::vec3(-1.0f,  0.0f,  0.0f),	 //left
		glm::vec3(0.0f,  0.0f,  1.0f), //forward
		glm::vec3(0.0f,  0.0f, -1.0f), //back
	};

	_elevation.Reset(1000000, -100000);
	for (int i = 0; i < 6; i++)
	{
		delete _faces[i];
		_faces[i] = new PlanetFace(_settings->Resolution, directions[i], *_settings, _elevation);
	}

	printf("Elevation: min: %f, max: %f \n", _elevation.Min, _elevation.Max);
}

//----- Planet Face -----
PlanetFace::PlanetFace(const int resolution, const glm::vec3& localUp, PlanetSettings& settings, MinMaxFloat& elevation)
{
	struct Vertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);
	};

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	const int vertexCount = resolution * resolution;
	const int indexCount = (resolution - 1) * (resolution - 1) * 6;
	vertices.resize(vertexCount);
	indices.resize(indexCount);

	const glm::vec3 axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
	const glm::vec3 axisB = glm::cross(localUp, axisA);

	for(int y = 0, vi = 0, ti = 0; y < resolution; y++)
	{
		for(int x = 0; x < resolution; x++, vi++)
		{
			const glm::vec2 factor = glm::vec2(x, y) / (float)(resolution - 1);
			glm::vec3 pointOnCube = localUp + (factor.x - 0.5f) * 2.0f * axisA + (factor.y - 0.5f) * 2.0f * axisB;
			glm::vec3 pointOnSphere = glm::normalize(pointOnCube);
			glm::vec3 pointOnPlanet = settings.CalculatePointOnPlanet(pointOnSphere, elevation);

			vertices[vi] = { pointOnPlanet };
			if (x == resolution - 1 || y == resolution - 1) continue;

			indices[ti    ] = vi;
			indices[ti + 1] = vi + resolution + 1;
			indices[ti + 2] = vi + resolution;

			indices[ti + 3] = vi;
			indices[ti + 4] = vi + 1;
			indices[ti + 5] = vi + resolution + 1;
			ti += 6;
		}
	}
	for(int i = 0; i < indexCount; i+=3)
	{
		//Triangle
		Vertex v1 = vertices[indices[i]];
		Vertex v2 = vertices[indices[i + 1]];
		Vertex v3 = vertices[indices[i + 2]];

		glm::vec3 normal = glm::normalize(glm::cross(v2.position - v1.position, v3.position - v1.position));
		v1.normal = v2.normal = v3.normal = normal;
	}

	const int vertexSize = sizeof(float) * 6;
	BufferLayout layout;
	layout.AddLayoutElement(3, GL_FLOAT, false, vertexSize, 0);
	layout.AddLayoutElement(3, GL_FLOAT, false, vertexSize, 3);
	_mesh = new Mesh(&vertices[0].position.x, vertexSize, vertexCount, &indices[0], indexCount, layout);
}

PlanetFace::~PlanetFace()
{
	delete _mesh;
}