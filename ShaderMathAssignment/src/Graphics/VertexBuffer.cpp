#include "VertexBuffer.h"



//------Vertex buffer
VertexBuffer::VertexBuffer()
{
	glCreateBuffers(1, &_bufferId);
}
VertexBuffer::VertexBuffer(const float* data, const int size)
{
	glCreateBuffers(1, &_bufferId);
	SetData(data, size);
}
void VertexBuffer::SetData(const float* data, const int size) const
{
	glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &_bufferId);
}
void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
}
void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::AddLayoutElement(const LayoutElement element)
{
	_layout.push_back(element);
}
void VertexBuffer::AddLayoutElement(GLuint size, GLuint type, bool normalized, GLuint stride, GLuint offset)
{
	_layout.push_back(LayoutElement{ size, type, normalized, stride, offset });
}

//------Index buffer

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &_indexBufferId);
}
IndexBuffer::IndexBuffer(const unsigned int* indices, const unsigned int count)
{
	glGenBuffers(1, &_indexBufferId);
	SetData(indices, count);
}

void IndexBuffer::SetData(const unsigned int* indices, const unsigned int count)
{
	_indexCount = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &_indexBufferId);
}
void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
}
void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
