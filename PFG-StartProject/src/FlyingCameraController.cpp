#include "FlyingCameraController.h"
#include "Scene.h"
#include "Camera.h"


void FlyingCameraController::Update(float deltaTs, Input* input)
{
	cam->_cameraAngleX -= input->getMouseDelta().x * cam->mouseSpeed;
	cam->_cameraAngleY -= input->getMouseDelta().y * cam->mouseSpeed;

	glm::vec3 direction(cos(cam->_cameraAngleY) * sin(cam->_cameraAngleX), sin(cam->_cameraAngleY), cos(cam->_cameraAngleY) * cos(cam->_cameraAngleX));
	glm::vec3 right = glm::vec3(sin(cam->_cameraAngleX - 3.14f / 2.0f), 0, cos(cam->_cameraAngleX - 3.14f / 2.0f));
	glm::vec3 up = glm::cross(right, direction);

	// Move camera in a direction
	if (input->cmd_w)
	{
		cam->_translation = direction * cam->speed;
		cam->_cameraPos += cam->_translation;

	}
	else if (input->cmd_s)
	{
		cam->_translation = -(direction * cam->speed);
		cam->_cameraPos += cam->_translation;
	}
	if (input->cmd_a)
	{
		cam->_translation = -(right * cam->speed);
		cam->_cameraPos += cam->_translation;

	}
	else if (input->cmd_d)
	{
		cam->_translation = right * cam->speed;
		cam->_cameraPos += cam->_translation;
	}
	if (input->cmd_x)
	{
		auto gos = scene->GetObjects();
		for (int i = 0; i < gos.size(); i++)
		{
			gos.at(i)->SetSimulated(true);
		}
	}
}

void FlyingCameraController::Initialize()
{
	cam = scene->GetCamera();
}
