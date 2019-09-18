#include "Texture.h"
#include <SOIL.h>

Texture::Texture(const float* pixels, const int width, const int height) : _width(width), _height(height)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &TextureId);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_FLOAT, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

Texture::Texture(const std::string& path)
{
	unsigned char* pixels = SOIL_load_image(path.c_str(), &_width, &_height, &_channels, SOIL_LOAD_RGBA);

	SwizzleRows(pixels);
	
	glCreateTextures(GL_TEXTURE_2D, 1, &TextureId);
	glBindTexture(GL_TEXTURE_2D, TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	SOIL_free_image_data(pixels);
}

Texture::~Texture()
{
	glDeleteTextures(1, &TextureId);
}
void Texture::Bind(const int slot) const
{
	glBindTextureUnit(slot, TextureId);
}
void Texture::SwizzleRows(unsigned char* data) const
{
	const int rowSize = _width * 4;
	auto* temp = new unsigned char[rowSize];
	const int iterations = _height / 2;
	for(int i = 0; i < iterations; i++)
	{
		unsigned char* source = data + rowSize * i;
		unsigned char* target = data + rowSize * (_height - i - 1);
		memcpy(temp, source, rowSize);
		memcpy(source, target, rowSize);
		memcpy(target, temp, rowSize);
	}
	delete[] temp;
}
