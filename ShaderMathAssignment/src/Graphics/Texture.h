#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture() = default;
	Texture(const float* pixels, const int width, const int height);
	Texture(const std::string& path);
	~Texture();
	void Bind(const int slot) const;
	GLuint TextureId;
private:
	void SwizzleRows(unsigned char* data) const;
	int _width, _height, _channels;
};

