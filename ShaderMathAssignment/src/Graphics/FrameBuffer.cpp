#include "FrameBuffer.h"
#include <GL/glew.h>

FrameBuffer::FrameBuffer(const int width, const int height)
{
	glGenFramebuffers(1, &_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);

	glGenTextures(1, &_colorTextureHandle);
	glBindTexture(GL_TEXTURE_2D, _colorTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _colorTextureHandle, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glGenTextures(1, &_depthTextureHandle);
	glBindTexture(GL_TEXTURE_2D, _depthTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthTextureHandle, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);



	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);
}
void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, _colorTextureHandle);
}
