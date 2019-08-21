#pragma once
#include <GL/glew.h>

class VertexBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void Bind();
	void Unbind();
	void AddVertexBuffer(VertexBuffer* vertexBuffer);
private:
	GLuint _arrayId;
};

