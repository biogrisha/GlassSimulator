#pragma once

#include <Views/RenderSpace/view_renderSpace.h>
#include <Modules/CameraModule.h>

class ModRel_Camera_RendSpace
{
public:
	ModRel_Camera_RendSpace(CameraModule* mod, view_renderSpace* view)
	{
		m_mod = mod;
		m_view = view;
		onMouseDrag.reset(m_view->events.onMouseDrag.AddHandler(&ModRel_Camera_RendSpace::onMouseDragProc, this));
		onForward.reset(m_view->events.onWKey.AddHandler(&ModRel_Camera_RendSpace::onForwardProc, this));
		onBack.reset(m_view->events.onSKey.AddHandler(&ModRel_Camera_RendSpace::onBackProc, this));
		onRight.reset(m_view->events.onDKey.AddHandler(&ModRel_Camera_RendSpace::onRightProc, this));
		onLeft.reset(m_view->events.onAKey.AddHandler(&ModRel_Camera_RendSpace::onLeftProc, this));
	}

	void onMouseDragProc(float x, float y)
	{
		m_mod->ChangeCameraDirection(x, y);
	}

	void onForwardProc(float deltaTime)
	{
		m_mod->MoveCamera(Camera_Movement::FORWARD, deltaTime);
	}
	void onBackProc(float deltaTime)
	{
		m_mod->MoveCamera(Camera_Movement::BACKWARD, deltaTime);
	}
	void onRightProc(float deltaTime)
	{
		m_mod->MoveCamera(Camera_Movement::RIGHT, deltaTime);
	}
	void onLeftProc(float deltaTime)
	{
		m_mod->MoveCamera(Camera_Movement::LEFT, deltaTime);
	}
	
private:

	EventHandlerRAII<float, float> onMouseDrag;
	EventHandlerRAII<float> onForward;
	EventHandlerRAII<float> onBack;
	EventHandlerRAII<float> onRight;
	EventHandlerRAII<float> onLeft;

	CameraModule* m_mod;
	view_renderSpace* m_view;
};