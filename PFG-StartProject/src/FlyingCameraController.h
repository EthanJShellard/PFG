#pragma once
#include "Script.h"

class Camera;

class FlyingCameraController : public Script 
{
	//Reference to camera in parent scene. Used as player's position and viewpoint.
	Camera* m_cam;

	void Update(float _deltaTs, Input* _input) override;
	void Initialize() override;
};