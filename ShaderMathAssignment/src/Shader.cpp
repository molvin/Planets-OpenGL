#include "Shader.h"
#include <vector>
#include "FileManager.h"

Shader::Shader(const std::string& vertexSource, const std::string& fragSource)
{
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, nullptr);
	glCompileShader(vertexShader);

	GLint compiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);

	if(compiled == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> infoLog(length);
		glGetShaderInfoLog(vertexShader, length, &length, &infoLog[0]);

		//TODO: add logger for error logging

		return;
	}

	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	source = fragSource.c_str();
	glShaderSource(fragmentShader, 1, &source, nullptr);
	glCompileShader(fragmentShader);

	compiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> infoLog(length);
		glGetShaderInfoLog(vertexShader, length, &length, &infoLog[0]);

		//TODO: add logger for error logging

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

		//TODO: add logger for error logging
		return;
	}

	glDetachShader(_programId, vertexShader);
	glDetachShader(_programId, fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(_programId);
}

void Shader::Bind() const
{
	glUseProgram(_programId);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}
