#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "VertexArray.h"
#include <glm/mat4x2.hpp>

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
	~Mesh();
	const VertexArray* GetVertexArray() const { return _vao; }
	static void LoadObj(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
private:
	VertexArray* _vao;
	VertexBuffer* _vbo;
	IndexBuffer* _ibo;
};

