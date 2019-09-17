#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "VertexArray.h"
#include "../Core/Transform.h"
#include "VertexBuffer.h"

class Shader;

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};
class Mesh
{
public:
	Mesh(const std::string& path);
	Mesh(float* vertices, const unsigned int vertexSize, const unsigned int vertexCount, unsigned int* indices, const unsigned int indexCount, const std::vector<LayoutElement> layout);
	const VertexArray* GetVertexArray() const { return &_vao; }

	static void LoadObj(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
protected:
	VertexArray _vao;
	VertexBuffer _vbo;
	IndexBuffer _ibo;
};

