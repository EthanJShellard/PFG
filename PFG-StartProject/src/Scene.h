#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

#include "GameObject.h"
#include "KinematicObject.h"
#include "DynamicObject.h"
#include "Camera.h"
#include "PerformanceMonitor.h"
#include "Script.h"
#include <vector>

/*! \brief Brief description.
*  Scene class is a container for loading all the game objects in your simulation or your game. 
*  
*/

class Scene
{
public:

	/** Scene constructor
	* Currently the scene is set up in the constructor
	* This means the object(s) are loaded, given materials and positions as well as the camera and light
	*/
	Scene();
	/** Scene distructor
	*/
	~Scene();

	/** Scene update
	* This function is called for each simulation time step to
	* update on all objects in the scene
	*/
	void Update(float _deltaTs, Input* _input);

	/** 
	* Call this function to get a pointer to the camera
	* 
	*/
    Camera* GetCamera() { return m_camera; }

	/** Draw the scene from the camera's point of view
	*
	*/
	void Draw();
	//Add _newObject into the scene so that it will be included in updates
	void AddObject(std::shared_ptr<GameObject> _newObject);
	//Delete all objects with ID _ID
	void DeleteObjectsByID(int _ID);
	//Get a vector of this scenes game objects
	std::vector<std::shared_ptr<GameObject>> GetObjects();
	//Set this scene's performance monitor
	void SetPerformanceMonitor(PerformanceMonitor* _monitor) { m_perfMonitor = _monitor; };
	//Add a script to this scene so that it will be included in updates
	void AddScript(std::shared_ptr<Script> _script);
	//Find a script with a specific ID
	std::shared_ptr<Script> GetScriptByID(int _ID);
	//Initialise all objects and scripts in the scene
	void Initialize();
	//Find an object with a specific ID
	std::shared_ptr<GameObject> FindObjectByID(int _ID);
private:
	//Vectors containing all gameobjects and scripts in this scene
	std::vector< std::shared_ptr<GameObject> > m_gameObjects;
	std::vector< std::shared_ptr<Script> > m_scripts;

	/** The main camera in the scene 
	*/
	Camera* m_camera; 

	/**The position for a light that lits the scene
	*/
	glm::vec3 m_lightPosition; 
	/** This matrix represents the camera's position and orientation 
	*/
	glm::mat4 m_viewMatrix; 
	/** This matrix is the camera's lens
	*/
	glm::mat4 m_projMatrix; 
	/** A boolen variable to control the start of the simulation This matrix is the camera's lens
	*/
	bool m_simulationStart;
	//This scee's performance monitor
	PerformanceMonitor* m_perfMonitor;
};

#endif // !_SCENE_H_