#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();
	void Bind(unsigned int slot = 0);
private:
	GLuint _textureId;
	int _width, _height, _channels;
};

