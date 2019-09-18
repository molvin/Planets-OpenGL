#pragma once
#include <string>
#include <tuple>
#include <GL/glew.h>
#include <glm/detail/type_mat4x4.hpp>
#include <map>

class Shader
{
public:
	Shader();
	Shader(const std::string& path);
	Shader(const std::string& vertexSource, const std::string& fragSource);
	~Shader();
	void Bind() const;
	void Unbind() const;
	//Uniform setters
	void UploadUniformFloat(const std::string& name, float value) const;
	void UploadUniformVec2(const std::string& name, glm::vec2 vec) const;
	void UploadUniformMat4(const std::string& name, glm::mat4 mat) const;
	void UploadUniformInt(const std::string& name, int i) const;
	void UploadUniformVec3(const std::string& name, const glm::vec3 vec);

	GLint GetUniformLocation(const std::string& name) const;
	static std::tuple<std::string, std::string> ParseShaderFile(const std::string& path);
private:
	void LoadShader(const std::string& vertexSource, const std::string& fragSource);
	GLuint _programId;
	mutable std::map<std::string, GLint> _uniformLocations;
};

