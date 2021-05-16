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
	Camera* m_cam;

	std::shared_ptr<Mesh> m_ballMesh;
	std::shared_ptr<Material> m_ballMaterial;
	std::vector< std::pair< std::shared_ptr<DynamicObject>, float> > m_balls;
	float m_timeout;

	float m_xBound;
	float m_zBound;


	void Update(float _deltaTs, Input* _input) override;
	void Initialize() override;
	std::shared_ptr<DynamicObject> CreateBall();
};