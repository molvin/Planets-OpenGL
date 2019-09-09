#pragma once
#include <string>
#include <GL/glew.h>

class CubeMap
{
public:
	CubeMap(const std::string directories[]);
	~CubeMap();
	GLuint Handle;
	void Bind(const int index);
};

