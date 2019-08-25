#pragma once
#include <string>
#include <tuple>
#include <GL/glew.h>
#include <glm/detail/type_mat4x4.hpp>

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
	void UploadUniformMat4(const std::string& name, glm::mat4 mat) const;
	void UploadUniformInt(const std::string& name, int i) const;
	static std::tuple<std::string, std::string> ParseShaderFile(const std::string& path);
private:
	GLuint _programId;
};

