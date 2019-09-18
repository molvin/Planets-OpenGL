#include "Material.h"
#include "Texture.h"


Material::Material(Shader* shader)
{
	_shader = shader;
}


void Material::Bind() const
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
	for (const std::tuple<const Texture*, int>& texture : _textures)
		std::get<0>(texture)->Bind(std::get<1>(texture));
}

void Material::AddTexture(const Texture* texture, const int slot)
{
	_textures.emplace_back(texture, slot);
}
void Material::SetUniform(const std::string& name, const std::variant<float, int, glm::vec2, glm::vec3, glm::mat4>& value)
{
	_uniforms[name] = value;
}
