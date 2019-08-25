#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/vec4.hpp>

struct LayoutElement
{
	GLuint size;
	GLuint type;
	bool normalized;
	GLuint stride;
	GLuint offset;
};

class BufferLayout
{
public:
	BufferLayout();
	~BufferLayout();
	void AddLayoutElement(GLuint size, GLuint type, bool normalized, GLuint stride, GLuint offset);
	const std::vector<LayoutElement>& GetLayout() const;
private:
	std::vector<LayoutElement> _layout;
};

class VertexBuffer
{
public:
	VertexBuffer(const float* data, const int size, const BufferLayout& layout);
	~VertexBuffer();
	void Bind();
	void Unbind();
	BufferLayout& GetLayout() { return _layout; }
private:
	GLuint _bufferId;
	BufferLayout _layout;
};

class IndexBuffer
{
public:
	IndexBuffer(const unsigned* indices, unsigned count);
	~IndexBuffer();
	void Bind();
	void Unbind();
	unsigned int GetCount() const { return _indexCount; }
private:
	GLuint _indexBufferId;
	unsigned int _indexCount;
};


