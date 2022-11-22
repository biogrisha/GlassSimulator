#pragma once
#include "../RenderUnit.h"
#include <Services/BatchRendering/FrameBuffer/ImageFrameBuffer.h>

class ru_clearTg : public RenderUnit
{
public:
	ru_clearTg(uint32_t frameViewId, ImageFrameBuffer* fb);
	virtual void Update() override;

private:
	ImageFrameBuffer* m_fb;
};