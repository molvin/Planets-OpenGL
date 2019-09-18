#pragma once
#include <string>
#include <map>
class Shader;

class ShaderLibrary
{
public:
	static void AddShader(const std::string& name, std::shared_ptr<Shader>& shader);
	static Shader* GetShader(const std::string& name);
private:
	static std::map<std::string, std::shared_ptr<Shader>> _shaders;
};

