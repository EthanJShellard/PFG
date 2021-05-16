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

	//Cached mesh and material to save memory and loading time when creating new balls
	std::shared_ptr<Mesh> m_ballMesh;
	std::shared_ptr<Material> m_ballMaterial;

	//Enemy sprite
	std::shared_ptr<GameObject> m_sprite;
	//Quad to display a press start message
	std::shared_ptr<GameObject> m_pressStart;
	
	//Vector of pairs which contain a ball and time it has been alive
	std::vector< std::pair<std::shared_ptr<DynamicObject>, float> > m_balls;
	//lifetime of balls
	float m_timeout;
	//Has game started?
	bool m_start;
	//Information to direct the enemy projectile balls
	glm::vec3 m_defaultThrowDirection;
	float m_maxThrowSpeed;
	float m_minThrowSpeed;
	float m_cooldownTimer;
	float m_cooldown; //Time between throws
	float m_maxVerticalDeviation; //In degrees
	float m_maxHorizontalDeviation; //In degrees
	
	//Horizontal bounds of movement for enemy
	float m_posXMin;
	float m_posXMax;
	
	bool m_movingRight;
	//Enemy speed
	float m_speed;

	//Create new ball projectile
	std::shared_ptr<DynamicObject> CreateBall();

	void Update(float _deltaTs, Input* _input) override;
	void Initialize() override;
	//Move player to death/game over screen and disable controls
	void ShowDeathScreen();
};
