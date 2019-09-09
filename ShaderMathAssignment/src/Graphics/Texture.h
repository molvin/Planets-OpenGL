#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture();
	Texture(const std::string& path, const unsigned int slot);
	~Texture();
	void Bind() const;
	void SetSlot(const unsigned int slot) { _slot = slot; }
	GLuint TextureId;
private:
	void SwizzleRows(unsigned char* data);
	unsigned int _slot;
	int _width, _height, _channels;
};

