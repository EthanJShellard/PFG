#pragma once
#include "Script.h"
#include <memory>
#include <vector>

class Camera;
class Mesh;
class Material;
class DynamicObject;

class BallThrowerController : public Script 
{
	//Reference to camera in parent scene. Used as player's position and viewpoint.
	Camera* m_cam;

	//Cached mesh and material to save memory and loading time when creating new balls
	std::shared_ptr<Mesh> m_ballMesh;
	std::shared_ptr<Material> m_ballMaterial;

	//Vector of pairs which contain a ball and time it has been alive
	std::vector< std::pair< std::shared_ptr<DynamicObject>, float> > m_balls;
	//Lifetime of balls
	float m_timeout;

	//Bounds of player horizontal movement
	float m_xBound;
	float m_zBound;


	void Update(float _deltaTs, Input* _input) override;
	void Initialize() override;
	std::shared_ptr<DynamicObject> CreateBall();
};