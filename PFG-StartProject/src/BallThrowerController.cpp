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

void BallThrowerController::Update(float deltaTs, Input* input)
{
	cam->_cameraAngleX -= input->getMouseDelta().x * cam->mouseSpeed;
	//Clamp vertical camera rotation
	cam->_cameraAngleY = std::max(std::min(cam->_cameraAngleY - input->getMouseDelta().y * cam->mouseSpeed, MAX_CAM_ANGLE), MIN_CAM_ANGLE);

	glm::vec3 right = glm::vec3(sin(cam->_cameraAngleX - 3.14f / 2.0f), 0, cos(cam->_cameraAngleX - 3.14f / 2.0f));
	glm::vec3 direction(cos(cam->_cameraAngleY) * sin(cam->_cameraAngleX), 0.0f, cos(cam->_cameraAngleY) * cos(cam->_cameraAngleX));
	glm::vec3 forward(cos(cam->_cameraAngleY) * sin(cam->_cameraAngleX), sin(cam->_cameraAngleY), cos(cam->_cameraAngleY) * cos(cam->_cameraAngleX));

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
	cam->_cameraPos.x = std::max(std::min(cam->_cameraPos.x, xBound), -xBound);
	cam->_cameraPos.z = std::max(std::min(cam->_cameraPos.z, xBound), -zBound);

	if (input->cmd_mouseleftUp) 
	{
		CreateBall()->SetVelocity(forward * 20.0f);
	}

	//WRITE TIMER TO DELETE BALLS
	for (int i = 0; i < balls.size(); i++) 
	{
		if (balls.at(i).second > timeout) 
		{
			balls.at(i).first->ID = -10;
			balls.erase(std::remove(balls.begin(), balls.end(), balls.at(i)), balls.end());
		}
		else
		{
			balls.at(i).second += deltaTs;
		}
	}
	scene->DeleteObjectsByID(-10);
}



void BallThrowerController::Initialize()
{
	cam = scene->GetCamera();

	xBound = 14;
	zBound = 10;
	timeout = 8.0f;

	//Load cached assets
	ballMesh = std::make_shared<Mesh>();
	ballMesh->LoadOBJ("assets/models/sphere.obj");

	ballMaterial = std::make_shared<Material>();
	ballMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	ballMaterial->SetDiffuseColour(glm::vec3(0.1, 0.8, 0.1));
	ballMaterial->SetTexture("assets/textures/default.bmp");
	ballMaterial->SetLightPosition(glm::vec3(0, 10, 0));
}

std::shared_ptr<DynamicObject> BallThrowerController::CreateBall()
{
	std::shared_ptr<DynamicObject> newBall = std::make_shared<DynamicObject>();
	newBall->SetMaterial(ballMaterial);
	newBall->SetMesh(ballMesh); 
	newBall->SetPosition(cam->_cameraPos);
	newBall->SetSimulated(true);
	newBall->SetScale(0.5f, 0.5f, 0.5f);
	newBall->SetMass(1.0f);
	newBall->SetCollider(std::make_shared<SphereCollider>(0.5f));
	newBall->GetCollider()->bounciness = 0.2f;
	newBall->GetCollider()->friction = 0.4f;
	newBall->InitialiseCollider();


	balls.push_back(std::pair< std::shared_ptr<DynamicObject>, float>(newBall, 0.0f));
	scene->AddObject(newBall);
	return newBall;
}
