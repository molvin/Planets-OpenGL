#pragma once
#include "Shader.h"
#include <queue>

class Material
{
public:
	Material(Shader* shader);
	~Material();
	void Bind();

	void SetUniformFloat(const std::string& name, float value);
	void SetUniformVec2(const std::string& name, const glm::vec2& vec);
	void SetUniformMat4(const std::string& name, const glm::mat4& mat);
	void SetUniformInt(const std::string& name, int i);
	Shader* GetShader() const { return _shader; }
private:
	Shader* _shader;
	std::map<std::string, float> _floatUniforms;
	std::map<std::string, int> _intUniforms;
	std::map<std::string, glm::vec2> _vec2Uniforms;
	std::map<std::string, glm::mat4> _mat4Uniforms;
	
};

