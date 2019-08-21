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


VertexBuffer::VertexBuffer(const float* data, const int size)
{
	_bufferId = 0;
	glCreateBuffers(1, &_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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


