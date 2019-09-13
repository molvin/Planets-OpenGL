#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& path);
	~Texture();
	void Bind(const int slot) const;
	GLuint TextureId;
private:
	void SwizzleRows(unsigned char* data);
	int _width, _height, _channels;
};

