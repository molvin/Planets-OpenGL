#pragma once
#include <GL/glew.h>
#include "VertexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;
	void SetBuffers(const VertexBuffer& vertexBuffer, IndexBuffer* indexBuffer);
	IndexBuffer* GetIndexBuffer() const { return _indexBuffer; }
private:
	GLuint _arrayId = 0;
	IndexBuffer* _indexBuffer = nullptr;
};

