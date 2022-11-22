#include "ru_clearTg.h"

ru_clearTg::ru_clearTg(uint32_t frameViewId, ImageFrameBuffer* fb) : RenderUnit(frameViewId)
{
	m_fb = fb;
}

void ru_clearTg::Update() 
{
	m_fb->Bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	m_fb->Unbind();
}
