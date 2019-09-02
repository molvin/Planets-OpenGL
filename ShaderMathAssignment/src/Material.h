#pragma once
#include "Shader.h"
#include <queue>
#include <variant>

class Material
{
public:
	Material(Shader* shader);
	~Material();
	void Bind();

	void SetUniform(const std::string& name, const std::variant<float, int, glm::vec2, glm::vec3, glm::mat4>& value);
	Shader* GetShader() const { return _shader; }
private:
	Shader* _shader;
	std::map<std::string, std::variant<float, int, glm::vec2, glm::vec3, glm::mat4>> _uniforms;
};

