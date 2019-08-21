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
	VertexBuffer(const float* data, const int size);
	~VertexBuffer();
	void Bind();
	void Unbind();
	void SetLayout(const BufferLayout& layout) { _layout = layout; }
	BufferLayout& GetLayout() { return _layout; }
private:
	GLuint _bufferId;
	BufferLayout _layout;
};

