#pragma once
#include "glm/glm.hpp"
#include "GameObject.h"
#include <memory>

enum class ColliderType { SPHERE, INFINITE_PLANE };

class GameObject;

struct Collider 
{
	//Current position
	glm::vec3 m_pos;
	//Next position if no collision - used for collision detection
	glm::vec3 m_nextPos;
	float m_bounciness;
	float m_friction;
	glm::vec3 m_velocity;
	glm::vec3 m_centreOfMass;
	float m_mass;

	glm::mat3 m_bodyInteriaTensor;
	glm::mat3 m_inverseBodyInertiaTensor;

	GameObject * m_parent;

	ColliderType GetType();
	virtual void ComputeCentreOfMass() = 0;
	glm::mat3 ComputeInverseInertiaTensor(glm::quat _rotation);
	Collider();
	virtual void Initialise() = 0;
	virtual void Update(float _deltaTs) = 0;

protected:
	ColliderType m_type;
};

struct SphereCollider : public Collider
{
	float m_radius;

	SphereCollider(float _radius);
	void ComputeCentreOfMass() override;
	void Initialise() override;
	void Update(float _deltaTs) override;
};

struct InfinitePlaneCollider : public Collider
{
	glm::vec3 m_planeNormal;
	glm::vec3 m_pointOnPlane;
	InfinitePlaneCollider(glm::vec3 _planeNormal, glm::vec3 _pointOnPlane);
	void ComputeCentreOfMass() override;
	void Initialise() override;
	void Update(float _deltaTs) override;
};