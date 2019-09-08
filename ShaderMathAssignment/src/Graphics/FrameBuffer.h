#pragma once
#include <GL/glew.h>
#include "Texture.h"

class FrameBuffer
{
public:
	FrameBuffer(const int width, const int height);
	void Bind();
	void Unbind();
	void BindTexture();
private:
	GLuint _handle;
	GLuint _colorTextureHandle;
	GLuint _depthTextureHandle;
};

