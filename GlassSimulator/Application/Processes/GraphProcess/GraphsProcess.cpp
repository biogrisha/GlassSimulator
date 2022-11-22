#include "GraphsProcess.h"

GraphsProcess::GraphsProcess()
{
  m_camerModule.reset(new CameraModule(&m_camera, &m_frameViewManager));
  m_renderModule.reset(new RenderModule(&m_camera, &m_frameViewManager));
  m_cameraRel.reset(new ModRel_Camera_RendSpace(m_camerModule.get(), &m_rendSpaceView));
}

void GraphsProcess::Run()
{
    m_renderModule->UpdateState();
	m_rendSpaceView.Show();
    
}


