#include "Shader.h"
#include <vector>
#include <sstream>
#include <fstream>
#include "glm/gtc/type_ptr.hpp"

Shader::Shader()
{
}
Shader::Shader(const std::string& path)
{
	auto[vert, frag] = ParseShaderFile(path);
	LoadShader(vert, frag);
}
Shader::Shader(const std::string& vertexSource, const std::string& fragSource)
{
	LoadShader(vertexSource, fragSource);
}
Shader::~Shader()
{
	glDeleteProgram(_programId);
	printf("Shader destroyed\n");
}
void Shader::Bind() const
{
	glUseProgram(_programId);
}
void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::UploadUniformFloat(const std::string& name, const float value) const
{
	glUniform1f(GetUniformLocation(name), value);
}
void Shader::UploadUniformVec2(const std::string& name, glm::vec2 vec) const
{
	glUniform2f(GetUniformLocation(name), vec.x, vec.y);
}
void Shader::UploadUniformMat4(const std::string& name, glm::mat4 mat) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::UploadUniformInt(const std::string& name, int i) const
{
	glUniform1i(GetUniformLocation(name), i);
}

void Shader::UploadUniformVec3(const std::string& name, const glm::vec3 vec)
{
	glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	if (_uniformLocations.find(name) != _uniformLocations.end())
		return _uniformLocations[name];

	const GLint location = glGetUniformLocation(_programId, name.c_str());
	_uniformLocations[name] = location;
	return location;
}

void Shader::LoadShader(const std::string& vertexSource, const std::string& fragSource)
{
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, nullptr);
	glCompileShader(vertexShader);
	//delete[] source;

	GLint compiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> infoLog(length);
		glGetShaderInfoLog(vertexShader, length, &length, &infoLog[0]);

		printf("Error compiling vertex shader: \n%s", &infoLog[0]);
		return;
	}

	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	source = fragSource.c_str();
	glShaderSource(fragmentShader, 1, &source, nullptr);
	glCompileShader(fragmentShader);
	//delete[] source;

	compiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> infoLog(length);
		glGetShaderInfoLog(fragmentShader, length, &length, &infoLog[0]);

		printf("Error compiling fragment shader: \n%s", &infoLog[0]);
		return;
	}

	_programId = glCreateProgram();
	glAttachShader(_programId, vertexShader);
	glAttachShader(_programId, fragmentShader);
	glLinkProgram(_programId);

	GLint isLinked = 0;
	glGetProgramiv(_programId, GL_LINK_STATUS, static_cast<int*>(&isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint length = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &length);

		std::vector<GLchar> infoLog(length);
		glGetProgramInfoLog(_programId, length, &length, &infoLog[0]);

		glDeleteProgram(_programId);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		printf("Error linking shader program: \n%s", &infoLog[0]);
		return;
	}

	printf("Shader compiled successfully\n");
	glDetachShader(_programId, vertexShader);
	glDetachShader(_programId, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


std::tuple<std::string, std::string> Shader::ParseShaderFile(const std::string& path)
{
	std::ifstream file(path);
	std::string line;

	enum ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type = NONE;
	std::stringstream stringStreams[2];

	while(std::getline(file, line))
	{
		if(line.find("#SHADER") != std::string::npos)
		{
			if (line.find("VERTEX") != std::string::npos)
				type = VERTEX;
			else if (line.find("FRAGMENT") != std::string::npos)
				type = FRAGMENT;
			else
			{
				printf("Failed to parse file: %s \n", path.c_str());
				return { "", "" };
			}
		}
		else
		{
			if(type == NONE)
			{
				printf("Failed to parse file: %s \n", path.c_str());
				return { "", "" };
			}
			stringStreams[type] << line << '\n';
		}
	}
	return { stringStreams[0].str(), stringStreams[1].str() };
}

