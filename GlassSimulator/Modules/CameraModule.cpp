#include "CameraModule.h"

CameraModule::CameraModule(Camera* camera, FrameViewManager* frameViewManager)
{
	m_camera = camera;
	m_frameViewManager = frameViewManager;

}

void CameraModule::MoveCamera(Camera_Movement direction, float deltaTime)
{
	m_camera->ProcessKeyboard(direction, deltaTime);
	m_frameViewManager->RequestUpdate();
}

void CameraModule::ChangeCameraDirection(float x, float y)
{
	m_camera->ProcessMouseMovement(x, y);
	m_frameViewManager->RequestUpdate();
}
