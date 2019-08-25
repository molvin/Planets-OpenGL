#include "Mesh.h"
#include <fstream>
#include <sstream>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include <algorithm>
#include "Renderer.h"

Mesh::Mesh(const std::string& path)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	LoadObj(path, vertices, normals, indices);
}
Mesh::~Mesh()
{
}

void Mesh::Render(const Shader* shader)
{
}

void Mesh::LoadObj(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices)
{
}

