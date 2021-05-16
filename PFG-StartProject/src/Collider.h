#pragma once
#include "glm/glm.hpp"
#include <memory>

class GameObject;

enum class ColliderType { SPHERE, INFINITE_PLANE };

struct Collider 
{
	//Current position
	glm::vec3 m_pos;
	//Next position if no collision - used for collision detection
	glm::vec3 m_nextPos;
	//Substiture for coefficient of restitution
	float m_bounciness;
	//Friction of this collider's surfaces
	float m_friction;
	//Velocity of this collider
	glm::vec3 m_velocity;
	//Centre of mass of this collider
	glm::vec3 m_centreOfMass;
	//Mass of this collider's parent
	float m_mass;

	//Inertia tensors for angular momentum calculation
	glm::mat3 m_bodyInteriaTensor;
	glm::mat3 m_inverseBodyInertiaTensor;

	//This collider's parent game object
	GameObject * m_parent;

	//Get the type of this collider
	ColliderType GetType();
	//Compute the centre of mass of this collider. Pure virtual, should be overriden by each collider type
	virtual void ComputeCentreOfMass() = 0;
	//Compute the reverse inertia tensor of this collider
	glm::mat3 ComputeInverseInertiaTensor(glm::quat _rotation);

	Collider();
	
	//Pure virtual function, should be overriden by each collider type. Sets up the the collider.
	virtual void Initialise() = 0;
	//Pure virtual function, should be overriden by each collider type, updates collider information before collision detection
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