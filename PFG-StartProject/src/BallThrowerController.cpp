#include "BallThrowerController.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "DynamicObject.h"
#include "Collider.h"

#include <iostream>

#define MAX_CAM_ANGLE 1.0f 
#define MIN_CAM_ANGLE -1.0f 

void BallThrowerController::Update(float _deltaTs, Input* _input)
{
	//Update camera angles
	m_cam->m_cameraAngleX -= _input->getMouseDelta().x * m_cam->m_mouseSpeed;
	//Clamp vertical camera rotation
	m_cam->m_cameraAngleY = std::max(std::min(m_cam->m_cameraAngleY - _input->getMouseDelta().y * m_cam->m_mouseSpeed, MAX_CAM_ANGLE), MIN_CAM_ANGLE);

	//Create camera directions
	glm::vec3 right = glm::vec3(sin(m_cam->m_cameraAngleX - 3.14f / 2.0f), 0, cos(m_cam->m_cameraAngleX - 3.14f / 2.0f));
	glm::vec3 direction(cos(m_cam->m_cameraAngleY) * sin(m_cam->m_cameraAngleX), 0.0f, cos(m_cam->m_cameraAngleY) * cos(m_cam->m_cameraAngleX));
	glm::vec3 forward(cos(m_cam->m_cameraAngleY) * sin(m_cam->m_cameraAngleX), sin(m_cam->m_cameraAngleY), cos(m_cam->m_cameraAngleY) * cos(m_cam->m_cameraAngleX));

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
	m_cam->m_cameraPos.x = std::max(std::min(m_cam->m_cameraPos.x, m_xBound), -m_xBound);
	m_cam->m_cameraPos.z = std::max(std::min(m_cam->m_cameraPos.z, m_xBound), -m_zBound);

	//Shoot ball
	if (_input->cmd_mouseleftUp) 
	{
		CreateBall()->SetVelocity(forward * 20.0f);
	}

	//TIMER TO DELETE BALLS
	for (int i = 0; i < m_balls.size(); i++) 
	{
		if (m_balls.at(i).second > m_timeout) 
		{
			m_balls.at(i).first->ID = -10;
			m_balls.erase(std::remove(m_balls.begin(), m_balls.end(), m_balls.at(i)), m_balls.end());
		}
		else
		{
			m_balls.at(i).second += _deltaTs;
		}
	}
	m_scene->DeleteObjectsByID(-10);
}



void BallThrowerController::Initialize()
{
	m_cam = m_scene->GetCamera();

	m_xBound = 14;
	m_zBound = 10;
	m_timeout = 8.0f;

	//Load cached assets
	m_ballMesh = std::make_shared<Mesh>();
	m_ballMesh->LoadOBJ("assets/models/sphere.obj");

	m_ballMaterial = std::make_shared<Material>();
	m_ballMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	m_ballMaterial->SetDiffuseColour(glm::vec3(0.1, 0.8, 0.1));
	m_ballMaterial->SetTexture("assets/textures/default.bmp");
	m_ballMaterial->SetLightPosition(glm::vec3(0, 10, 0));
}

std::shared_ptr<DynamicObject> BallThrowerController::CreateBall()
{
	std::shared_ptr<DynamicObject> newBall = std::make_shared<DynamicObject>();
	newBall->SetMaterial(m_ballMaterial);
	newBall->SetMesh(m_ballMesh); 
	newBall->SetPosition(m_cam->m_cameraPos);
	newBall->SetSimulated(true);
	newBall->SetScale(0.5f, 0.5f, 0.5f);
	newBall->SetMass(1.0f);
	newBall->SetCollider(std::make_shared<SphereCollider>(0.5f));
	newBall->GetCollider()->m_bounciness = 0.2f;
	newBall->GetCollider()->m_friction = 0.4f;
	newBall->InitialiseCollider();


	m_balls.push_back(std::pair< std::shared_ptr<DynamicObject>, float>(newBall, 0.0f));
	m_scene->AddObject(newBall);
	return newBall;
}
