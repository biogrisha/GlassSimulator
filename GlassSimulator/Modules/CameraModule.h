#pragma once
#include <Services/3DAuxiliaryFunctions/Camera.h>
#include <Services/BatchRendering/FrameViewManager.h>
class CameraModule
{
public:
	CameraModule(Camera* camera,FrameViewManager* frameViewManager);

	void MoveCamera(Camera_Movement direction, float deltaTime);
	void ChangeCameraDirection(float x, float y);

private:
	Camera* m_camera;
	FrameViewManager* m_frameViewManager;
};