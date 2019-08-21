#pragma once
#include <string>
#include <GL/glew.h>

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& vertexSource, const std::string& fragSource);
	~Shader();
	void Bind() const;
	void Unbind() const;
	//Uniform setters
	void UploadUniformFloat(const std::string& name, float value) const;
	void UploadUniformVec2(const std::string& name, float x, float y) const;
	static ShaderSource ParseShaderFile(const std::string& path);
private:
	GLuint _programId;
};

