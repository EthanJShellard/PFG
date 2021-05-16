#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GLM/glm.hpp> // This is the main GLM header
#include <GLM/gtc/matrix_transform.hpp> // This one lets us use matrix transformations
#include "Input.h"

/*! \brief Brief description.
*  Camera class sets up parameters for camera. It controls camera's movement based on keyboard and mourse information.
*
*/
class Camera
{
public:
	/** Camera constructor
	*/
	Camera();
	/** Camera distructor
	*/
	~Camera();
	/** Function for getting the viewing matrix
	* @return a 4x4 matrix
	*/
	inline glm::mat4 GetView() { return m_viewMatrix; };
	/** Function for getting the projection matrix
	* @return a 4x4 matrix
	*/
	inline glm::mat4 GetProj() { return m_projMatrix; };
	/** Function for getting the camera position
	* @param glm::vec3 pos camera position 
	*/
	inline void SetPos(glm::vec3 pos) { m_cameraPos = pos; }

	inline glm::vec3 GetPos() { return m_cameraPos; }

	/** Function for updatting the camera 
	* @param input keyboard and mourse input inofrmation
	*/
	void Update(Input* input);

	/** Camera's rotation angle in a radian about x axis
	*/
	float m_cameraAngleX;
	/** Camera's rotation angle in a radian about y axis
	*/
	float m_cameraAngleY;
	/** Camera's speed of movement
	*/
	float m_speed;
	/** The speed of the mouse movement
	*/
	float m_mouseSpeed;
	/** Camera's up vector
	*/
	glm::vec3 m_up;

	/** Camera's transaltion vector
	*/
	glm::vec3 m_translation;


	/** The state of the mouse position
	*/
	int m_posx, m_posy;
	/** The state of the mouse's previous position
	*/
	glm::vec2 m_oldMousePosition;
	glm::vec3 m_cameraPos;
	/** Position of the camera
	*/
	/** Orientation of the camera
	*/
	glm::vec3 m_cameraRot;
	/** Point that camera looks at
	*/
private:
	/** Viewing matrix for the camera
	*/
	glm::mat4 m_viewMatrix;
	/** Projection matrix for the camera
	*/
	glm::mat4 m_projMatrix;	

	glm::vec3 m_lookAt;
	

};

#endif // !_CAMERA_H_