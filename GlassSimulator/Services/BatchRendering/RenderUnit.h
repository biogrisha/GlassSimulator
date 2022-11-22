#pragma once
#include <iostream>
class  RenderUnit
{
public:
	virtual~RenderUnit() = default;
	RenderUnit(uint32_t frameViewId)
	{
		m_frameViewId = frameViewId;
	}
	virtual void Update()=0;
protected:
	uint32_t m_frameViewId = 0;
};