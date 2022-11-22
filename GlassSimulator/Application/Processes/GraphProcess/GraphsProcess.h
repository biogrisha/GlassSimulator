#pragma once
#include <Modules/CameraModule.h>
#include <Modules/RenderModule.h>
#include <Services/BatchRendering/FrameViewManager.h>
#include <Views/RenderSpace/view_renderSpace.h>
#include "../Relations/ModRel_Camera_RendSpace.h"
#include <vector>

class GraphsProcess
{
public:
	GraphsProcess();
	void Run();
private:
	
	FrameViewManager m_frameViewManager;
	Camera m_camera;
	view_renderSpace m_rendSpaceView;
	std::unique_ptr<CameraModule> m_camerModule;
	std::unique_ptr<ModRel_Camera_RendSpace> m_cameraRel;
	std::unique_ptr<RenderModule> m_renderModule;
	

};