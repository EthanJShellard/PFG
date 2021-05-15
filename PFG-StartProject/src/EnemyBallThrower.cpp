#include "EnemyBallThrower.h"
#include "Scene.h"
#include "GameObject.h"
#include "DynamicObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include <iostream>

std::shared_ptr<DynamicObject> EnemyBallThrower::CreateBall()
{
	std::shared_ptr<DynamicObject> newBall = std::make_shared<DynamicObject>();
	newBall->SetMaterial(ballMaterial);
	newBall->SetMesh(ballMesh);
	newBall->SetPosition(sprite->GetPosition());
	newBall->SetSimulated(true);
	newBall->SetScale(1.0f, 1.0f, 1.0f);
	newBall->SetMass(3.0f);
	newBall->SetCollider(std::make_shared<SphereCollider>(1.0f));
	newBall->GetCollider()->bounciness = 0.45f;
	newBall->GetCollider()->friction = 0.05f;
	newBall->InitialiseCollider();

	balls.push_back(std::pair< std::shared_ptr<DynamicObject>, float>(newBall, 0.0f));
	scene->AddObject(newBall);
	return newBall;
}

void EnemyBallThrower::Update(float deltaTs, Input* input)
{
	if (input->cmd_x) 
	{
		start = true;
		pressStart->ID = -11; //flag for delete at the end of this update
	}

	if(start)
	{
		if (movingRight)
		{
			sprite->SetPosition(sprite->GetPosition() + glm::vec3(1, 0, 0) * speed * deltaTs);
			if (sprite->GetPosition().x >= posXMax) movingRight = false;
		}
		else
		{
			sprite->SetPosition(sprite->GetPosition() + glm::vec3(-1, 0, 0) * speed * deltaTs);
			if (sprite->GetPosition().x <= posXMin) movingRight = true;
		}

		if (cooldownTimer <= 0)
		{
			auto ball = CreateBall();
			glm::vec3 throwDir = defaultThrowDirection;
			int horizontalSign = (std::rand() % 2) ? 1 : -1;
			int verticalSign = (std::rand() % 2) ? 1 : -1;

			throwDir = glm::angleAxis(glm::radians(verticalSign * (((float)std::rand() / RAND_MAX) * maxVerticalDeviation)), glm::vec3(1, 0, 0)) * throwDir;
			throwDir = glm::angleAxis(glm::radians(horizontalSign * (((float)std::rand() / RAND_MAX) * maxHorizontalDeviation)), glm::vec3(0, 1, 0)) * throwDir;
			ball->SetVelocity((minThrowSpeed + (((float)std::rand() / RAND_MAX) * maxThrowSpeed)) * throwDir);

			cooldownTimer = cooldown;
		}

		cooldownTimer -= deltaTs;

		glm::vec3 yZapper = glm::vec3(1, 0, 1);
		//WRITE TIMER TO DELETE BALLS
		for (int i = 0; i < balls.size(); i++)
		{
			if (balls.at(i).second > timeout)
			{
				balls.at(i).first->ID = -11;
				balls.erase(std::remove(balls.begin(), balls.end(), balls.at(i)), balls.end());
			}
			else
			{
				//Check for collision with player
				if (glm::distance(yZapper * balls.at(i).first->GetPosition(), yZapper * scene->GetCamera()->_cameraPos) < 1.5f)
				{
					std::cout << "DEAD" << std::endl;
				}

				balls.at(i).second += deltaTs;
			}
		}
		scene->DeleteObjectsByID(-11);
	}
	
}

void EnemyBallThrower::Initialize()
{
	cam = scene->GetCamera();
	start = false;

	timeout = 10.0f;

	defaultThrowDirection = glm::vec3(0,0,1);
	maxVerticalDeviation = 50.0f;
	maxHorizontalDeviation = 50.0f;
	maxThrowSpeed = 20.0f;
	minThrowSpeed = 10.0f;
	cooldown = 1.5f;
	cooldownTimer = 0.0f;
	posXMin = -10;
	posXMax = 10;
	movingRight = true;
	speed = 4.0f;

	//Load enemy sprite plane
	sprite = std::make_shared<GameObject>();
	sprite->SetPosition(0.0f,2.5f,-50.0f);
	sprite->SetRotation(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
	sprite->SetScale(0.5f,0.5f,0.5f);
	sprite->Rotate(glm::radians(180.0f), glm::vec3(0,0,1));

	auto spriteMesh = std::make_shared<Mesh>();
	spriteMesh->LoadOBJ("assets/models/woodfloor.obj");
	sprite->SetMesh(spriteMesh);

	auto spriteMaterial = std::make_shared<Material>();
	spriteMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	spriteMaterial->SetDiffuseColour(glm::vec3(1.0,1.0,1.0));
	spriteMaterial->SetTexture("assets/textures/imp.bmp");
	spriteMaterial->SetLightPosition(glm::vec3(0,10,0));
	sprite->SetMaterial(spriteMaterial);

	scene->AddObject(sprite);
	/////////////////////////////

	//Load press start plane
	pressStart = std::make_shared<GameObject>();
	pressStart->SetPosition(glm::vec3(0.0f,4.5f,-25.0f));
	pressStart->SetRotation(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
	pressStart->SetScale(1.0f, 1.0f, 1.0f);
	pressStart->Rotate(glm::radians(180.0f), glm::vec3(0, 0, 1));
	pressStart->Rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));

	auto pressStartMesh = std::make_shared<Mesh>();
	pressStartMesh->LoadOBJ("assets/models/woodfloor.obj");
	pressStart->SetMesh(pressStartMesh);

	auto pressStartMaterial = std::make_shared<Material>();
	pressStartMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	pressStartMaterial->SetDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
	pressStartMaterial->SetTexture("assets/textures/pressstart.bmp");
	pressStartMaterial->SetLightPosition(glm::vec3(0,5,5));
	pressStart->SetMaterial(pressStartMaterial);

	scene->AddObject(pressStart);
	///////////////////////////////

	//Load cached assets
	ballMesh = std::make_shared<Mesh>();
	ballMesh->LoadOBJ("assets/models/sphere.obj");

	ballMaterial = std::make_shared<Material>();
	ballMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	ballMaterial->SetDiffuseColour(glm::vec3(0.8, 0.1, 0.1));
	ballMaterial->SetTexture("assets/textures/default.bmp");
	ballMaterial->SetLightPosition(glm::vec3(0, 10, 0));
}
