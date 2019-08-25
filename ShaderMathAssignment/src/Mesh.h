#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "VertexArray.h"

class Shader;

class Mesh
{
public:
	Mesh(const std::string& path);
	~Mesh();
	void Render(const Shader* shader);
	static void LoadObj(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices);
};

