#pragma once
#include <GL/glew.h>

class IndexBuffer;
class VertexBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;
	void AddVertexBuffer(VertexBuffer* vertexBuffer) const;
	void SetIndexBuffer(IndexBuffer* indexBuffer);
	IndexBuffer* GetIndexBuffer() const { return _indexBuffer; }
private:
	GLuint _arrayId;
	IndexBuffer* _indexBuffer;
};

