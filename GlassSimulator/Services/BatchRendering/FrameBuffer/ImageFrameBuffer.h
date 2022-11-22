#pragma once
#include <glad/glad.h>

class ImageFrameBuffer
{
public:
	ImageFrameBuffer();
	~ImageFrameBuffer();

	void SetTextureAttachment(GLenum attachment, GLuint texture, GLenum target = GL_FRAMEBUFFER);
	void SetRenderBuffer();
	void Bind(GLenum target = GL_FRAMEBUFFER);
	void Unbind(GLenum target = GL_FRAMEBUFFER);
	bool IsComplete();
private:
	unsigned int m_framebuffer;
	unsigned int rbo;
};