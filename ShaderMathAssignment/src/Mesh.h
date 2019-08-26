#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "VertexArray.h"
#include <glm/mat4x2.hpp>

class Shader;

class Mesh
{
public:
	Mesh(const std::string& path);
	~Mesh();
	void Render(const Shader* shader, const glm::mat4& transform) const;
	static void LoadObj(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices);
private:
	static void ReadValuesFromLine(std::stringstream& ss, float* values, const int count);

	VertexArray* _vao;
	VertexBuffer* _vbo;
	IndexBuffer* _ibo;
};

