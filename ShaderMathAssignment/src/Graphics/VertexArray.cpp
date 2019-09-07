#include "VertexArray.h"
#include "VertexBuffer.h"


VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &_arrayId);
}
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_arrayId);
}
void VertexArray::Bind() const
{
	glBindVertexArray(_arrayId);
}
void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer) const
{
	Bind();
	vertexBuffer->Bind();

	const BufferLayout layout = vertexBuffer->GetLayout();
	const int size = layout.GetLayout().size();
	for(int i = 0; i < size; i++)
	{
		const LayoutElement element = layout.GetLayout()[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.size, element.type, element.normalized, element.stride, reinterpret_cast<void*>(element.offset));
	}	
}
void VertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
{
	Bind();

	indexBuffer->Bind();
	_indexBuffer = indexBuffer;
}

