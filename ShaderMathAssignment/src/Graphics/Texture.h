#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& path, const unsigned int slot);
	~Texture();
	void Bind() const;
private:
	void SwizzleRows(unsigned char* data);
	GLuint _textureId;
	unsigned int _slot;
	int _width, _height, _channels;
};

