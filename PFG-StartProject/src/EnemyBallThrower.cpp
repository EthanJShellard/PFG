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
	newBall->SetMaterial(m_ballMaterial);
	newBall->SetMesh(m_ballMesh);
	newBall->SetPosition(m_sprite->GetPosition());
	newBall->SetSimulated(true);
	newBall->SetScale(1.0f, 1.0f, 1.0f);
	newBall->SetMass(3.0f);
	newBall->SetCollider(std::make_shared<SphereCollider>(1.0f));
	newBall->GetCollider()->m_bounciness = 0.45f;
	newBall->GetCollider()->m_friction = 0.05f;
	newBall->InitialiseCollider();

	m_balls.push_back(std::pair< std::shared_ptr<DynamicObject>, float>(newBall, 0.0f));
	m_scene->AddObject(newBall);
	return newBall;
}

void EnemyBallThrower::Update(float _deltaTs, Input* _input)
{
	if (_input->cmd_x) 
	{
		m_start = true;
		m_pressStart->ID = -11; //flag for delete at the end of this update
	}

	if(m_start)
	{
		if (m_movingRight)
		{
			m_sprite->SetPosition(m_sprite->GetPosition() + glm::vec3(1, 0, 0) * m_speed * _deltaTs);
			if (m_sprite->GetPosition().x >= m_posXMax) m_movingRight = false;
		}
		else
		{
			m_sprite->SetPosition(m_sprite->GetPosition() + glm::vec3(-1, 0, 0) * m_speed * _deltaTs);
			if (m_sprite->GetPosition().x <= m_posXMin) m_movingRight = true;
		}

		if (m_cooldownTimer <= 0)
		{
			auto ball = CreateBall();
			glm::vec3 throwDir = m_defaultThrowDirection;
			int horizontalSign = (std::rand() % 2) ? 1 : -1;
			int verticalSign = (std::rand() % 2) ? 1 : -1;

			throwDir = glm::angleAxis(glm::radians(verticalSign * (((float)std::rand() / RAND_MAX) * m_maxVerticalDeviation)), glm::vec3(1, 0, 0)) * throwDir;
			throwDir = glm::angleAxis(glm::radians(horizontalSign * (((float)std::rand() / RAND_MAX) * m_maxHorizontalDeviation)), glm::vec3(0, 1, 0)) * throwDir;
			ball->SetVelocity((m_minThrowSpeed + (((float)std::rand() / RAND_MAX) * m_maxThrowSpeed)) * throwDir);

			m_cooldownTimer = m_cooldown;
			m_cooldown = max(m_cooldown *= 0.99f, 0.1f);
		}

		m_cooldownTimer -= _deltaTs;

		glm::vec3 yZapper = glm::vec3(1, 0, 1);
		//WRITE TIMER TO DELETE BALLS
		for (int i = 0; i < m_balls.size(); i++)
		{
			if (m_balls.at(i).second > m_timeout)
			{
				m_balls.at(i).first->ID = -11;
				m_balls.erase(std::remove(m_balls.begin(), m_balls.end(), m_balls.at(i)), m_balls.end());
			}
			else
			{
				//Check for collision with player
				if (glm::distance(yZapper * m_balls.at(i).first->GetPosition(), yZapper * m_scene->GetCamera()->m_cameraPos) < 1.5f)
				{
					m_scene->GetScriptByID(1)->m_enabled = false; //Disable player control class
					ShowDeathScreen();
				}

				m_balls.at(i).second += _deltaTs;
			}
		}
		m_scene->DeleteObjectsByID(-11);
	}
	
}

void EnemyBallThrower::Initialize()
{
	m_cam = m_scene->GetCamera();
	m_start = false;

	m_timeout = 10.0f;

	m_defaultThrowDirection = glm::vec3(0,0,1);
	m_maxVerticalDeviation = 50.0f;
	m_maxHorizontalDeviation = 50.0f;
	m_maxThrowSpeed = 20.0f;
	m_minThrowSpeed = 10.0f;
	m_cooldown = 1.5f;
	m_cooldownTimer = 0.0f;
	m_posXMin = -10;
	m_posXMax = 10;
	m_movingRight = true;
	m_speed = 4.0f;

	//Load enemy sprite plane
	m_sprite = std::make_shared<GameObject>();
	m_sprite->SetPosition(0.0f,2.5f,-50.0f);
	m_sprite->SetRotation(glm::vec3(glm::radians(-90.0f), glm::radians(180.0f), 0.0f));
	m_sprite->SetScale(0.5f,0.5f,0.5f);

	auto spriteMesh = std::make_shared<Mesh>();
	spriteMesh->LoadOBJ("assets/models/woodfloor.obj");
	m_sprite->SetMesh(spriteMesh);

	auto spriteMaterial = std::make_shared<Material>();
	spriteMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	spriteMaterial->SetDiffuseColour(glm::vec3(1.0,1.0,1.0));
	spriteMaterial->SetTexture("assets/textures/imp.bmp");
	spriteMaterial->SetLightPosition(glm::vec3(0,10,0));
	m_sprite->SetMaterial(spriteMaterial);

	m_scene->AddObject(m_sprite);
	/////////////////////////////

	//Load press start plane
	m_pressStart = std::make_shared<GameObject>();
	m_pressStart->SetPosition(glm::vec3(0.0f,4.5f,-25.0f));
	m_pressStart->SetRotation(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
	m_pressStart->SetScale(1.0f, 1.0f, 1.0f);

	auto pressStartMesh = std::make_shared<Mesh>();
	pressStartMesh->LoadOBJ("assets/models/woodfloor.obj");
	m_pressStart->SetMesh(pressStartMesh);

	auto pressStartMaterial = std::make_shared<Material>();
	pressStartMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	pressStartMaterial->SetDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
	pressStartMaterial->SetTexture("assets/textures/pressstart.bmp");
	pressStartMaterial->SetLightPosition(glm::vec3(0,5,-5));
	m_pressStart->SetMaterial(pressStartMaterial);

	m_scene->AddObject(m_pressStart);
	///////////////////////////////

	//Load cached assets
	m_ballMesh = std::make_shared<Mesh>();
	m_ballMesh->LoadOBJ("assets/models/sphere.obj");

	m_ballMaterial = std::make_shared<Material>();
	m_ballMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	m_ballMaterial->SetDiffuseColour(glm::vec3(0.8, 0.1, 0.1));
	m_ballMaterial->SetTexture("assets/textures/default.bmp");
	m_ballMaterial->SetLightPosition(glm::vec3(0, 10, 0));
}

void EnemyBallThrower::ShowDeathScreen()
{
	m_cam->m_cameraAngleX = 0;
	m_cam->m_cameraAngleY = 0;
	m_cam->m_cameraPos = glm::vec3(200, 0, 0);
	std::cout << "Game Over! Press escape to exit." << std::endl;
}
