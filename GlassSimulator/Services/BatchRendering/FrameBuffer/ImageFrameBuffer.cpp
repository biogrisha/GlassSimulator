#include "ImageFrameBuffer.h"

ImageFrameBuffer::ImageFrameBuffer()
{
	glGenFramebuffers(1, &m_framebuffer);
}

ImageFrameBuffer::~ImageFrameBuffer()
{
	glDeleteFramebuffers(1, &m_framebuffer);
}

void ImageFrameBuffer::SetTextureAttachment(GLenum attachment, GLuint texture, GLenum target)
{
	Bind(GL_FRAMEBUFFER);
	glFramebufferTexture2D(target, attachment, GL_TEXTURE_2D, texture, 0);
	Unbind(GL_FRAMEBUFFER);
}

void ImageFrameBuffer::SetRenderBuffer()
{
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

void ImageFrameBuffer::Bind(GLenum target)
{
	glBindFramebuffer(target, m_framebuffer);
}

void ImageFrameBuffer::Unbind(GLenum target)
{
	glBindFramebuffer(target, 0);
}

bool ImageFrameBuffer::IsComplete()
{
	Bind(GL_FRAMEBUFFER);
	bool val = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	Unbind(GL_FRAMEBUFFER);
	return val;
}
