#pragma once
#include "Script.h"

class Camera;

class FlyingCameraController : public Script 
{
	Camera* cam;

	void Update(float deltaTs, Input* input) override;
	void Initialize() override;
};