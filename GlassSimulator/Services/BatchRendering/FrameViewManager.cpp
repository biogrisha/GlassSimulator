#include "FrameViewManager.h"

FrameViewManager::~FrameViewManager()
{
	m_isActive = false;
}

FrameViewManager::FrameViewManager()
{

}

FrameViewBase* FrameViewManager::AddFrameView(std::unique_ptr<FrameViewBase>&& frameViewPtr)
{
	frameViewPtr->SetIdInArray(m_frameViews.size());
	m_frameViews.push_back(std::move(frameViewPtr));
	return m_frameViews.back().get();
}

void FrameViewManager::DeleteFrameView(FrameViewBase** frameViewPtr)
{

	uint32_t id = (*frameViewPtr)->GetIdInArray();//Get pos of frame view in the array of framevies
	*frameViewPtr = nullptr; // assign null to pointer
	if (id == m_frameViews.size() - 1) //if id is the last lement then just pop
	{
		m_frameViews.pop_back();
	}
	else 
	{
		FrameViewBase* lastEl = m_frameViews.back().get();//else get last fv
		lastEl->SetIdInArray(id); //set its id to that of fv to delete
		std::iter_swap(m_frameViews.begin() + id, m_frameViews.end() - 1);//swap
		m_frameViews.pop_back();//pop 
	}
	
}

void FrameViewManager::ProcessAll(GLBuffer* glBuffer)
{

	if(!m_frameViews.empty() && m_isActive && m_updateRequested)
	{
		glBindVertexArray(glBuffer->GetVAO());
		glBuffer->Bind();
		for (auto& frame : m_frameViews)
		{
			frame->UpdateFrame();
		}
		glBindVertexArray(0);
		glBuffer->Unbind();

		m_updateRequested = false;
		int i = 0;
	}
}

bool FrameViewManager::IsActive()
{
	return m_isActive;
}

FrameViewBase::~FrameViewBase()
{
	
}

FrameViewBase::FrameViewBase(GLsizei width, GLsizei height)
{
	m_fb_width = width;
	m_fb_height = height;
}

void FrameViewBase::SetIdInArray(uint32_t id)
{
	m_idInArray = id;
}

uint32_t FrameViewBase::GetIdInArray()
{
	return m_idInArray;
}

uint32_t FrameViewBase::GetId()
{
	return m_id;
}

void FrameViewBase::SetSize(GLsizei width, GLsizei height)
{
	m_fb_width = width;
	m_fb_height = height;
}

void FrameViewBase::UpdateFrame()
{
	
	GLint aiViewport[4];
	glGetIntegerv(GL_VIEWPORT, aiViewport);
	glViewport(0, 0, m_fb_width, m_fb_height);
	
	for (auto& ru : m_renderUnits)
	{
		ru->Update();
	}
	glViewport(aiViewport[0], aiViewport[1], (GLsizei)aiViewport[2], (GLsizei)aiViewport[3]);
}

void FrameViewBase::AddRenderUnit(std::unique_ptr<RenderUnit>&& renderUnit)
{
	m_renderUnits.push_back(std::move(renderUnit));
}



