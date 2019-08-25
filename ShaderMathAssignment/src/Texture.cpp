#include "Texture.h"
#include <SOIL.h>

Texture::Texture(const std::string& path)
{
	unsigned char* pixels = SOIL_load_image(path.c_str(), &_width, &_height, &_channels, SOIL_LOAD_RGBA);

	glCreateTextures(GL_TEXTURE_2D, 1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	SOIL_free_image_data(pixels);
}
Texture::~Texture()
{
	glDeleteTextures(1, &_textureId);
}
void Texture::Bind(unsigned slot)
{
	glBindTextureUnit(slot, _textureId);
}
