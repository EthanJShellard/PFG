#pragma once
#include "Scene.h"
#include "PFGCollision.h"


/*! \brief Brief description.
*  Scene class is a container for loading all the game objects in your simulation or your game.
*
*/
Scene::Scene()
{
	// Set up your scene here......
	// Set a camera
	m_camera = new Camera();
	// Don't start simulation yet
	m_simulationStart = false;

}

Scene::~Scene()
{
	// You should neatly clean everything up here
	//delete _physics_object;
	//delete _physics_object2;
	delete m_camera;
}

void Scene::Update(float _deltaTs, Input* _input)
{
	//Update scripts
	for (int i = 0; i < m_scripts.size(); i++) 
	{
		if(m_scripts.at(i)->m_enabled)m_scripts.at(i)->Update(_deltaTs, _input);
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->UpdateCollider(_deltaTs);
	}

	//CHECK COLLISION
	bool didCollide = false;
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		if (!m_gameObjects.at(i)->GetCollider()) continue; //No need to perform checks if no collider
		m_gameObjects.at(i)->ClearCollisions();

		//Collision checks for this object begin, begin monitoring
		m_perfMonitor->CollisionBegin();

		for (int j = 0; j < m_gameObjects.size(); j++) 
		{
			if (i == j || !m_gameObjects.at(j)->GetCollider()) continue; //No need to perform checks if no collider

			didCollide = false;

			Collision c = PFG::CheckCollision(m_gameObjects.at(i)->GetCollider(), m_gameObjects.at(j)->GetCollider(), didCollide);

			if (didCollide) m_gameObjects.at(i)->AddCollision(c);
		}
		//Collision checks for this object over, end monitoring
		m_perfMonitor->CollisionEnd();
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		//Update for this object beginning, start monitoring
		m_perfMonitor->UpdatesBegin();

		m_gameObjects.at(i)->Update(_deltaTs);

		//Update for this object ending, end monitoring
		m_perfMonitor->UpdatesEnd();
	}

	//_physics_object->Update(deltaTs);
	//_physics_object2->Update(deltaTs);
	//_level->Update(deltaTs);
	m_camera->Update(_input);

	m_viewMatrix = m_camera->GetView();
	m_projMatrix = m_camera->GetProj();
														
}

void Scene::Draw()
{
	// Draw objects, giving the camera's position and projection
	//_physics_object->Draw(_viewMatrix, _projMatrix);
	//_physics_object2->Draw(_viewMatrix, _projMatrix);
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Draw(m_viewMatrix, m_projMatrix);
	}
	//_level->Draw(_viewMatrix, _projMatrix);

}

void Scene::AddObject(std::shared_ptr<GameObject> _newObject)
{
	m_gameObjects.push_back(_newObject);
}

void Scene::DeleteObjectsByID(int _ID)
{
	for (int i = 0; i < m_gameObjects.size(); i++) 
	{
		if (m_gameObjects.at(i)->ID == _ID) 
		{
			m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), m_gameObjects.at(i)), m_gameObjects.end());
		}
	}
}

std::vector<std::shared_ptr<GameObject>> Scene::GetObjects()
{
	return m_gameObjects;
}

std::shared_ptr<GameObject> Scene::GetObjectByID(int _ID)
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects.at(i)->ID == _ID) return m_gameObjects.at(i);
	}
}

void Scene::AddScript(std::shared_ptr<Script> _script)
{
	_script->m_scene = this;
	m_scripts.push_back(_script);
}

std::shared_ptr<Script> Scene::GetScriptByID(int _ID)
{
	for (int i = 0; i < m_scripts.size(); i++) 
	{
		if (m_scripts.at(i)->m_ID == _ID) return m_scripts.at(i);
	}
}

void Scene::Initialize()
{
	//Initialise all colliders
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->InitialiseCollider();
	}

	for (int i = 0; i < m_scripts.size(); i++) 
	{
		m_scripts.at(i)->Initialize();
	}
}

std::shared_ptr<GameObject> Scene::FindObjectByID(int _ID)
{
	for (int i = 0; i < m_gameObjects.size(); i++) 
	{
		if (m_gameObjects.at(i)->ID == _ID) 
		{
			return m_gameObjects.at(i);
		}
	}
	return nullptr;
}


