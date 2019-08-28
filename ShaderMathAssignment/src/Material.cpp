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

	for(auto const&[name,value] : _floatUniforms)
		_shader->UploadUniformFloat(name, value);
	for (auto const&[name, value] : _vec2Uniforms)
		_shader->UploadUniformVec2(name, value);
	for (auto const&[name, value] : _mat4Uniforms)
		_shader->UploadUniformMat4(name, value);
	for (auto const&[name, value] : _intUniforms)
		_shader->UploadUniformInt(name, value);

}
void Material::SetUniformFloat(const std::string& name, float value)
{
	_floatUniforms[name] = value;
}
void Material::SetUniformVec2(const std::string& name, const glm::vec2& vec)
{
	_vec2Uniforms[name] = vec;
}
void Material::SetUniformMat4(const std::string& name, const glm::mat4& mat)
{
	_mat4Uniforms[name] = mat;
}
void Material::SetUniformInt(const std::string& name, int i)
{
	_intUniforms[name] = i;
}
