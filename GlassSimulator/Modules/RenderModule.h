#pragma once
#include <Modules/CameraModule.h>
#include <Services/BatchRendering/FrameViewManager.h>
#include <Services/BatchRendering/FrameBuffer/ImageFrameBuffer.h>

#include <Services/BatchRendering/RenderUnits/ru_clearTg.h>
#include <Services/BatchRendering/RenderUnits/ru_drawGlass.h>

class RenderModule
{
public:
	RenderModule(Camera* camera, FrameViewManager* fvManager);
	~RenderModule();
	void UpdateState();
private:
	FrameViewManager* m_fvManager;
	Camera* m_camera;

	std::unique_ptr <GLBuffer> m_glBuf;
	ImageFrameBuffer fb;
	GLuint m_renderTarget;
	GLuint m_depthBuffer;

	FrameViewBase* m_frameView;
	
	

};