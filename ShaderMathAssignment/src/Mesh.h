#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "VertexArray.h"
#include <glm/mat4x2.hpp>
#include "Material.h"
#include "Transform.h"

class BufferLayout;
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
	Mesh(float* vertices, const unsigned int vertexSize, const unsigned int vertexCount, unsigned int* indices, const unsigned int indexCount, const BufferLayout& layout);
	~Mesh();
	void Init(float* vertices, const unsigned int vertexSize, const unsigned int vertexCount, unsigned int* indices, const unsigned int indexCount, const BufferLayout& layout);
	void SetMaterial(Material* material) { _material = material; }
	const VertexArray* GetVertexArray() const { return _vao; }
	Transform* GetTransform() { return &_transform; }
	const Material* GetMaterial() const { return _material; }
	static void LoadObj(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
private:
	VertexArray* _vao;
	VertexBuffer* _vbo;
	IndexBuffer* _ibo;
	Material* _material;
	Transform _transform;
};

