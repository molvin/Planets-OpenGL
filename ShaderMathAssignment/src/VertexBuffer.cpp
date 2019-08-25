#include "VertexBuffer.h"


BufferLayout::BufferLayout()
{	
}
BufferLayout::~BufferLayout()
{
}
void BufferLayout::AddLayoutElement(const GLuint size, const GLuint type, const bool normalized, const GLuint stride, const GLuint offset)
{
	_layout.push_back(LayoutElement{ size, type, normalized, stride, offset });
}
const std::vector<LayoutElement>& BufferLayout::GetLayout() const
{
	return _layout;
}

//------Vertex buffer

VertexBuffer::VertexBuffer(const float* data, const int size, const BufferLayout& layout)
{
	glCreateBuffers(1, &_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	_layout = layout;
}
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &_bufferId);
}
void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
}
void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//------Index buffer

IndexBuffer::IndexBuffer(const unsigned int* indices, const unsigned int count)
{
	_indexCount = count;
	glGenBuffers(1, &_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &_indexBufferId);
}
void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
}
void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
