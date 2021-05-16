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
	Camera* m_cam;
	std::shared_ptr<Mesh> m_ballMesh;
	std::shared_ptr<Material> m_ballMaterial;
	std::shared_ptr<GameObject> m_sprite;
	std::shared_ptr<GameObject> m_pressStart;
	std::vector< std::pair<std::shared_ptr<DynamicObject>, float> > m_balls;
	float m_timeout;

	bool m_start;
	glm::vec3 m_defaultThrowDirection;
	float m_maxThrowSpeed;
	float m_minThrowSpeed;
	float m_cooldownTimer;
	float m_cooldown;
	float m_maxVerticalDeviation; //In degrees
	float m_maxHorizontalDeviation; //In degrees
	float m_posXMin;
	float m_posXMax;
	bool m_movingRight;
	float m_speed;

	std::shared_ptr<DynamicObject> CreateBall();
	void Update(float _deltaTs, Input* _input) override;
	void Initialize() override;
	void ShowDeathScreen();
};
