#include "Mesh.h"
#include <fstream>
#include <sstream>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include <algorithm>
#include "Renderer.h"
#include <iostream>

Mesh::Mesh(const std::string& path)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	LoadObj(path, vertices, uvs, normals, indices);

	unsigned int cubeIndices[] =
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
	BufferLayout layout;
	layout.AddLayoutElement(3, GL_FLOAT, false, 0, 0);

	_vao = new VertexArray();
	_vbo = new VertexBuffer(&vertices[0].x, sizeof(float) * 8 * 24, layout);
	_ibo = new IndexBuffer(cubeIndices, 3 * 2 * 6);
	_vao->AddVertexBuffer(_vbo);
	_vao->SetIndexBuffer(_ibo);
}
Mesh::~Mesh()
{
	delete(_vao);
	delete(_vbo);
	delete(_ibo);
}

void Mesh::Render(const Shader* shader, const glm::mat4& transform) const
{
	Renderer::Render(shader, _vao, transform);
}

void Mesh::LoadObj(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices)
{
	std::ifstream file(path);
	std::string line;
	std::stringstream ss;

	while (std::getline(file, line))
	{
		ss << line;

		if (line.find("vn") == 0)
		{
			float normal[3];
			ReadValuesFromLine(ss, normal, 3);
			printf("Normals(x: %f, y: %f, z: %f) \n", normal[0], normal[1], normal[2]);
			normals.emplace_back(normal[0], normal[1], normal[2]);
		}
		else if (line.rfind("vt") == 0)
		{
			float uv[2];
			ReadValuesFromLine(ss, uv, 2);
			printf("UVs(u: %f, v: %f) \n", uv[0], uv[1]);
			uvs.emplace_back(uv[0], uv[1]);
		}
		else if (line.rfind('v') == 0)
		{
			float vertex[3];
			ReadValuesFromLine(ss, vertex, 3);

			printf("Vertex(x: %f, y: %f, z: %f) \n", vertex[0], vertex[1], vertex[2]);
			vertices.emplace_back(vertex[0] - 1, vertex[1] - 1, vertex[2] - 1);
		}
		else if (line.rfind('f') == 0)
		{
			//unsigned int index[9];
			//ReadValuesFromLine(ss, index, 9);
			//printf("Indices(%d/%d/%d/%d/%d/%d/%d/%d/%d) \n", index[0], index[1], index[2], index[3], index[4], index[5], index[6], index[7], index[8]);
			
		}
		ss.clear();
	}
}

void Mesh::ReadValuesFromLine(std::stringstream& ss, float* values, const int count)
{
	std::string temp;
	float f;
	int i = 0;
	while (!ss.eof() && i < count)
	{
		ss >> temp;
		if (std::stringstream(temp) >> f)
		{
			values[i] = f;
			i++;
		}
		temp = "";
	}
}