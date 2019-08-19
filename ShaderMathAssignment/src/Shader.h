#pragma once
#include <string>
#include <GL/glew.h>

class Shader
{
public:
	Shader(const std::string& vertexSource, const std::string& fragSource);
	~Shader();
	void Bind() const;
	void Unbind() const;
private:
	GLuint _programId;
};

