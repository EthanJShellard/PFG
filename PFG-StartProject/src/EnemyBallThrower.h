#pragma once
#include "Script.h"
#include "glm/glm.hpp"
#include <memory>
#include <vector>

class Camera;
class Mesh;
class Material;
class DynamicObject;
class GameObject;

class EnemyBallThrower : public Script
{
	Camera* cam;
	std::shared_ptr<Mesh> ballMesh;
	std::shared_ptr<Material> ballMaterial;
	std::shared_ptr<GameObject> sprite;
	std::shared_ptr<GameObject> pressStart;
	std::vector< std::pair<std::shared_ptr<DynamicObject>, float> > balls;
	float timeout;

	bool start;
	glm::vec3 defaultThrowDirection;
	float maxThrowSpeed;
	float minThrowSpeed;
	float cooldownTimer;
	float cooldown;
	float maxVerticalDeviation; //In degrees
	float maxHorizontalDeviation; //In degrees
	float posXMin;
	float posXMax;
	bool movingRight;
	float speed;

	std::shared_ptr<DynamicObject> CreateBall();
	void Update(float deltaTs, Input* input) override;
	void Initialize() override;
	void ShowDeathScreen();
};
