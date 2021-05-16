#pragma once
#include "Script.h"

class Camera;

class FlyingCameraController : public Script 
{
	Camera* m_cam;

	void Update(float _deltaTs, Input* _input) override;
	void Initialize() override;
};