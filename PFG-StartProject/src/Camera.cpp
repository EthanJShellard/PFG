#include "Camera.h"

/*! \brief Brief description.
*  Camera class sets up parameters for camera. It controls camera's movement based on keyboard and mourse information.
*
*/
Camera::Camera()
{
	m_cameraPos = glm::vec3(0.0f, 3.0f, 0.0f); //glm::vec3(4.0f, 3.0f, 3.0f);
	glm::vec3 initTarget = glm::vec3(-100.0, 0.0f, 0.0f);

	m_cameraAngleX = 90.0f;
	m_cameraAngleY = 0.0f;

	m_speed = 0.1f;
	m_mouseSpeed = 0.005f;

	m_oldMousePosition = glm::vec2(0.0, 0.0);

	m_up = glm::vec3(0, 1, 0); 

	//Build  projection and viewing matrices
	m_projMatrix = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
	m_viewMatrix = glm::lookAt(m_cameraPos, initTarget, m_up);
}

Camera::~Camera() {}

void Camera::Update(Input* _input)
{
	glm::vec3 direction(cos(m_cameraAngleY) * sin(m_cameraAngleX), sin(m_cameraAngleY), cos(m_cameraAngleY) * cos(m_cameraAngleX));
	glm::vec3 right = glm::vec3(sin(m_cameraAngleX - 3.14f / 2.0f), 0, cos(m_cameraAngleX - 3.14f / 2.0f));
	glm::vec3 up = glm::cross(right, direction);

	// the matrices used to see the models are changed.
	//_projMatrix = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 10000.0f);
	m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + direction, up);

}