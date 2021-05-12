#include "Camera.h"

/*! \brief Brief description.
*  Camera class sets up parameters for camera. It controls camera's movement based on keyboard and mourse information.
*
*/
Camera::Camera()
{
	_cameraPos = glm::vec3(0.0f, 3.0f, 0.0f); //glm::vec3(4.0f, 3.0f, 3.0f);
	glm::vec3 initTarget = glm::vec3(-100.0, 0.0f, 0.0f);

	_cameraAngleX = 90.0f;
	_cameraAngleY = 0.0f;

	speed = 0.1f;
	mouseSpeed = 0.005f;

	oldMousePosition = glm::vec2(0.0, 0.0);

	_up = glm::vec3(0, 1, 0); 

	//Build  projection and viewing matrices
	_projMatrix = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
	_viewMatrix = glm::lookAt(_cameraPos, initTarget, _up);
}

Camera::~Camera() {}

void Camera::Update(Input* input)
{
	glm::vec3 direction(cos(_cameraAngleY) * sin(_cameraAngleX), sin(_cameraAngleY), cos(_cameraAngleY) * cos(_cameraAngleX));
	glm::vec3 right = glm::vec3(sin(_cameraAngleX - 3.14f / 2.0f), 0, cos(_cameraAngleX - 3.14f / 2.0f));
	glm::vec3 up = glm::cross(right, direction);

	// the matrices used to see the models are changed.
	//_projMatrix = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 10000.0f);
	_viewMatrix = glm::lookAt(_cameraPos, _cameraPos + direction, up);

}