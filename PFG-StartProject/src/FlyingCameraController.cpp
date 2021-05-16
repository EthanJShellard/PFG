#include "FlyingCameraController.h"
#include "Scene.h"
#include "Camera.h"

void FlyingCameraController::Update(float _deltaTs, Input* _input)
{
	m_cam->m_cameraAngleX -= _input->getMouseDelta().x * m_cam->m_mouseSpeed;
	m_cam->m_cameraAngleY -= _input->getMouseDelta().y * m_cam->m_mouseSpeed;

	glm::vec3 direction(cos(m_cam->m_cameraAngleY) * sin(m_cam->m_cameraAngleX), sin(m_cam->m_cameraAngleY), cos(m_cam->m_cameraAngleY) * cos(m_cam->m_cameraAngleX));
	glm::vec3 right = glm::vec3(sin(m_cam->m_cameraAngleX - 3.14f / 2.0f), 0, cos(m_cam->m_cameraAngleX - 3.14f / 2.0f));
	glm::vec3 up = glm::cross(right, direction);

	// Move camera in a direction
	if (_input->cmd_w)
	{
		m_cam->m_translation = direction * m_cam->m_speed;
		m_cam->m_cameraPos += m_cam->m_translation;

	}
	else if (_input->cmd_s)
	{
		m_cam->m_translation = -(direction * m_cam->m_speed);
		m_cam->m_cameraPos += m_cam->m_translation;
	}
	if (_input->cmd_a)
	{
		m_cam->m_translation = -(right * m_cam->m_speed);
		m_cam->m_cameraPos += m_cam->m_translation;

	}
	else if (_input->cmd_d)
	{
		m_cam->m_translation = right * m_cam->m_speed;
		m_cam->m_cameraPos += m_cam->m_translation;
	}
	
}

void FlyingCameraController::Initialize()
{
	m_cam = m_scene->GetCamera();
}
