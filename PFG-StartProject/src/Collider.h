#pragma once
#include "glm/glm.hpp"
#include "GameObject.h"
#include <memory>

enum class ColliderType { SPHERE, INFINITE_PLANE };

class GameObject;

struct Collider 
{
	//Current position
	glm::vec3 pos;
	//Next position if no collision - used for collision detection
	glm::vec3 nextPos;
	float bounciness;
	glm::vec3 velocity;
	glm::vec3 centreOfMass;
	float mass;

	glm::mat3 bodyInteriaTensor;
	glm::mat3 inverseBodyInertiaTensor;


	GameObject * parent;

	ColliderType GetType();
	virtual void ComputeCentreOfMass() = 0;
	glm::mat3 ComputeInverseInertiaTensor(glm::quat rotation);
	Collider();
	virtual void Initialise() = 0;
	virtual void Update(float deltaTs) = 0;

protected:
	ColliderType type;
};

struct SphereCollider : public Collider
{
	float radius;

	SphereCollider(float radius, float bounciness);
	void ComputeCentreOfMass() override;
	void Initialise() override;
	void Update(float deltaTs) override;
};

struct InfinitePlaneCollider : public Collider
{
	glm::vec3 planeNormal;
	glm::vec3 pointOnPlane;
	InfinitePlaneCollider(glm::vec3 _planeNormal, glm::vec3 _pointOnPlane);
	void ComputeCentreOfMass() override;
	void Initialise() override;
	void Update(float deltaTs) override;
};