#include "ShaderLibrary.h"
#include "Shader.h"

std::map<std::string, std::shared_ptr<Shader>> ShaderLibrary::_shaders;


void ShaderLibrary::AddShader(const std::string& name, std::shared_ptr<Shader>& shader)
{
	_shaders[name] = shader;
}

Shader* ShaderLibrary::GetShader(const std::string& name)
{
	return _shaders[name].get();
}
