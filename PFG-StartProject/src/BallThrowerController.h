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
	Camera* cam;

	std::shared_ptr<Mesh> ballMesh;
	std::shared_ptr<Material> ballMaterial;
	std::vector< std::pair< std::shared_ptr<DynamicObject>, float> > balls;
	float timeout;

	float xBound;
	float zBound;


	void Update(float deltaTs, Input* input) override;
	void Initialize() override;
	std::shared_ptr<DynamicObject> CreateBall();
};