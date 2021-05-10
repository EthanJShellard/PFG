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

	// Position of the light, in world-space
	_lightPosition = glm::vec3(0, 10, 0);

	// Create a game object
	std::shared_ptr<DynamicObject> smallSphere = std::make_shared<DynamicObject>();
	std::shared_ptr<DynamicObject> bigSphere = std::make_shared<DynamicObject>();
	std::shared_ptr<GameObject> wallEast = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> wallNorth = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> wallWest = std::make_shared<GameObject>();
	// Create a game level object
	_level = std::make_shared<GameObject>();

	// Create the material for the game object- level
	Material *modelMaterial = new Material();
	// Shaders are now in files
	modelMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	modelMaterial->SetDiffuseColour(glm::vec3(0.8, 0.8, 0.8));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	modelMaterial->SetTexture("assets/textures/diffuse.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	modelMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material
	_level->SetMaterial(modelMaterial);
	wallEast->SetMaterial(modelMaterial);
	wallWest->SetMaterial(modelMaterial);
	wallNorth->SetMaterial(modelMaterial);

	// The mesh is the geometry for the object
	Mesh *groundMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	groundMesh->LoadOBJ("assets/models/woodfloor.obj");
	// Tell the game object to use this mesh
	_level->SetMesh(groundMesh);
	_level->SetPosition(0.0f, 0.0f, 0.0f);
	_level->SetRotation(0.0f, 0.0f, 0.0f);
	_level->SetScale(1.5f, 1.0f, 1.5f);
	wallEast->SetMesh(groundMesh);
	wallEast->SetPosition(7.5f, 7.5f, 0.0f);
	wallEast->SetRotation(glm::radians(90.0f), glm::radians(90.0f), 0.0f);
	wallEast->SetScale(1.5f, 1.0f, 1.5f);
	wallWest->SetMesh(groundMesh);
	wallWest->SetPosition(-7.5f, 7.5f, 0.0f);
	wallWest->SetRotation(glm::radians(90.0f), glm::radians(-90.0f), 0.0f);
	wallWest->SetScale(1.5f, 1.0f, 1.5f);
	wallNorth->SetMesh(groundMesh);
	wallNorth->SetPosition(0.0f, 7.5f, -7.5f);
	wallNorth->SetRotation(glm::radians(90.0f), 0.0f, 0.0f);
	wallNorth->SetScale(1.5f, 1.0f, 1.5f);

	// Create the material for the game object- level
	Material *objectMaterial = new Material();
	Material* objectMaterial2 = new Material();
	// Shaders are now in files
	objectMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	objectMaterial->SetDiffuseColour(glm::vec3(0.8, 0.1, 0.1));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	objectMaterial->SetTexture("assets/textures/default.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	objectMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material
	smallSphere->SetMaterial(objectMaterial);

	objectMaterial2->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	objectMaterial2->SetDiffuseColour(glm::vec3(0.1, 0.8, 0.1));
	objectMaterial2->SetTexture("assets/textures/default.bmp");
	objectMaterial2->SetLightPosition(_lightPosition);

	bigSphere->SetMaterial(objectMaterial2);
	


	// Set the geometry for the object
	Mesh *modelMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("assets/models/sphere.obj");
	// Tell the game object to use this mesh
	smallSphere->SetMesh(modelMesh);
	smallSphere->SetPosition(-1.0f, 5.0f, 0.0f);
	smallSphere->SetScale(0.5f, 0.5f, 0.5f);
	smallSphere->SetVelocity(glm::vec3(4.0f, 0.0f, 0.0f));
	smallSphere->SetMass(2.0f);

	bigSphere->SetMesh(modelMesh);
	bigSphere->SetPosition(1.0f, 5.0f, 0.0f);
	bigSphere->SetScale(0.5f, 0.5f, 0.5f);
	bigSphere->SetVelocity(glm::vec3(-4.0f, 0.0f, 0.0f));
	bigSphere->SetMass(2.0f);

	//CREATE COLLIDERS
	std::shared_ptr<InfinitePlaneCollider> levelCollider = std::make_shared<InfinitePlaneCollider>(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
	levelCollider->bounciness = 0.2;
	_level->SetCollider(levelCollider);
	std::shared_ptr<InfinitePlaneCollider> wallWestCollider = std::make_shared<InfinitePlaneCollider>(glm::vec3(1, 0, 0), wallWest->GetPosition());
	wallWestCollider->bounciness = 0.2;
	wallWest->SetCollider(wallWestCollider);
	std::shared_ptr<InfinitePlaneCollider> wallEastCollider = std::make_shared<InfinitePlaneCollider>(glm::vec3(-1, 0, 0), wallEast->GetPosition());
	wallEastCollider->bounciness = 0.2;
	wallEast->SetCollider(wallEastCollider);
	std::shared_ptr<InfinitePlaneCollider> wallNorthCollider = std::make_shared<InfinitePlaneCollider>(glm::vec3(0, 0, -1), wallNorth->GetPosition());
	wallNorthCollider->bounciness = 0.2;
	wallNorth->SetCollider(wallNorthCollider);

	std::shared_ptr<SphereCollider> smallSphereCollider = std::make_shared<SphereCollider>(smallSphere->GetScale().x, 0.5f);
	smallSphere->SetCollider(smallSphereCollider);

	std::shared_ptr<SphereCollider> bigSphereCollider = std::make_shared<SphereCollider>(bigSphere->GetScale().x , 0.5f);
	bigSphere->SetCollider(bigSphereCollider);
	/////////////////////////////////

	GetCamera()->SetPos(GetCamera()->GetPos() + glm::vec3(0.0, 0.0, 10.0));

	gameObjects.push_back(smallSphere);
	gameObjects.push_back(bigSphere);
	gameObjects.push_back(_level);
	gameObjects.push_back(wallEast);
	gameObjects.push_back(wallWest);
	gameObjects.push_back(wallNorth);

	for (int i = 0; i < gameObjects.size(); i++) 
	{
		gameObjects.at(i)->InitialiseCollider();
	}
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
	_level->Update(deltaTs);
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
	_level->Draw(_viewMatrix, _projMatrix);

}


