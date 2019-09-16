#pragma once
#include <GL/glew.h>
#include <vector>

struct LayoutElement
{
	GLuint size;
	GLuint type;
	bool normalized;
	GLuint stride;
	GLuint offset;
};

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const float* data, const int size);
	void SetData(const float* data, const int size) const;
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
	void AddLayoutElement(LayoutElement element);
	void AddLayoutElement(GLuint size, GLuint type, bool normalized, GLuint stride, GLuint offset);
	const std::vector<LayoutElement>& GetLayout() const { return _layout; }
private:
	GLuint _bufferId = 0;
	std::vector<LayoutElement> _layout;
};

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(const unsigned int* indices, const unsigned int count);
	void SetData(const unsigned int* indices, const unsigned int count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	unsigned int GetCount() const { return _indexCount; }
private:
	GLuint _indexBufferId = 0;
	unsigned int _indexCount = 0;
};


