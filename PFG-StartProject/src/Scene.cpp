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
	_camera = new Camera();
	// Don't start simulation yet
	_simulation_start = false;

}

Scene::~Scene()
{
	// You should neatly clean everything up here
	//delete _physics_object;
	//delete _physics_object2;
	delete _camera;
}

void Scene::Update(float deltaTs, Input* input)
{

	if (input->cmd_x)
	{
		//_physics_object->SetSimulated(true);
		//_physics_object2->SetSimulated(true);
		for (int i = 0; i < gameObjects.size(); i++) 
		{
			gameObjects.at(i)->SetSimulated(true);
		}
	}

	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.at(i)->UpdateCollider(deltaTs);
	}

	//CHECK COLLISION
	bool didCollide = false;
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (!gameObjects.at(i)->GetCollider()) continue; //No need to perform checks if no collider
		gameObjects.at(i)->ClearCollisions();

		for (int j = 0; j < gameObjects.size(); j++) 
		{
			if (i == j || !gameObjects.at(j)->GetCollider()) continue; //No need to perform checks if no collider

			didCollide = false;

			Collision c = PFG::CheckCollision(gameObjects.at(i)->GetCollider(), gameObjects.at(j)->GetCollider(), didCollide);

			if (didCollide) gameObjects.at(i)->AddCollision(c);
		}
	}

	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.at(i)->Update(deltaTs);
	}

	//_physics_object->Update(deltaTs);
	//_physics_object2->Update(deltaTs);
	//_level->Update(deltaTs);
	_camera->Update(input);

	_viewMatrix = _camera->GetView();
	_projMatrix = _camera->GetProj();
														
}

void Scene::Draw()
{
	// Draw objects, giving the camera's position and projection
	//_physics_object->Draw(_viewMatrix, _projMatrix);
	//_physics_object2->Draw(_viewMatrix, _projMatrix);
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.at(i)->Draw(_viewMatrix, _projMatrix);
	}
	//_level->Draw(_viewMatrix, _projMatrix);

}

void Scene::AddObject(std::shared_ptr<GameObject> newObject)
{
	gameObjects.push_back(newObject);
}

std::vector<std::shared_ptr<GameObject>> Scene::GetObjects()
{
	return gameObjects;
}


