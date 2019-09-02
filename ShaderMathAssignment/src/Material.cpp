#include "Material.h"



Material::Material(Shader* shader)
{
	_shader = shader;
}
Material::~Material()
{
}

void Material::Bind()
{
	_shader->Bind();
	for(auto const&[name, data] : _uniforms)
	{
		if (auto value = std::get_if<float>(&data))
			_shader->UploadUniformFloat(name, *value);
		else if (auto value = std::get_if<int>(&data))
			_shader->UploadUniformInt(name, *value);
		else if (auto value = std::get_if<glm::vec2>(&data))
			_shader->UploadUniformVec2(name, *value);
		else if (auto value = std::get_if<glm::vec3>(&data))
			_shader->UploadUniformVec3(name, *value);
		else if (auto value = std::get_if<glm::mat4>(&data))
			_shader->UploadUniformMat4(name, *value);
	}
}
void Material::SetUniform(const std::string& name, const std::variant<float, int, glm::vec2, glm::vec3, glm::mat4>& value)
{
	_uniforms[name] = value;
}
