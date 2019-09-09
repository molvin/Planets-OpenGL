#pragma once
#include <GL/glew.h>
#include "Texture.h"

class FrameBuffer
{
public:
	FrameBuffer(const int width, const int height);
	void Bind();
	void Unbind();
	const Texture* GetDepthTexture() const { return &_depthTexture; }
	const Texture* GetColorTexture() const { return &_colorTexture; }
private:
	GLuint _handle;
	Texture _colorTexture;
	Texture _depthTexture;
};

