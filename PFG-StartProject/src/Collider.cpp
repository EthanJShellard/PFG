#include "Collider.h"


ColliderType Collider::GetType()
{
	return type;
}

glm::mat3 Collider::ComputeInverseInertiaTensor(glm::quat rotation)
{
	glm::mat3 R = glm::toMat3(rotation);
	return R * inverseBodyInertiaTensor * glm::transpose(R);
}

Collider::Collider()
{
	pos = glm::vec3(0);
	nextPos = glm::vec3(0);
	bounciness = 0;
	parent = nullptr;
	mass = 0;
	type = ColliderType::SPHERE;
}

SphereCollider::SphereCollider(float _radius, float _bounciness)
{
	radius = _radius;
	bounciness = _bounciness;

	type = ColliderType::SPHERE;
}

void SphereCollider::ComputeCentreOfMass()
{
	centreOfMass = pos;
}

void SphereCollider::Initialise()
{
	mass = parent->GetMass();

	//COMPUTE INERTIA TENSORS
	//Solid Sphere
	bodyInteriaTensor = glm::mat3(
		(2.0f / 5.0f) * mass * radius * radius, 0.0f, 0.0f,
		0.0f, (2.0f / 5.0f) * mass * radius * radius, 0.0f,
		0.0f, 0.0f, (2.0f / 5.0f) * mass * radius * radius);
	//Inverse
	inverseBodyInertiaTensor = glm::inverse(bodyInteriaTensor);
}

void SphereCollider::Update(float deltaTs)
{
	pos = parent->GetPosition();
	velocity = parent->GetVelocity();
	nextPos = pos + (velocity * deltaTs);
}

InfinitePlaneCollider::InfinitePlaneCollider(glm::vec3 _planeNormal, glm::vec3 _pointOnPlane)
{
	type = ColliderType::INFINITE_PLANE;
	planeNormal = _planeNormal;
	pointOnPlane = _pointOnPlane;
}

void InfinitePlaneCollider::ComputeCentreOfMass()
{
	centreOfMass = pos;
}

void InfinitePlaneCollider::Initialise()
{
	mass = parent->GetMass();
	bodyInteriaTensor = glm::mat3();
	inverseBodyInertiaTensor = glm::mat3();
}

void InfinitePlaneCollider::Update(float deltaTs)
{
	pos = parent->GetPosition();
	velocity = parent->GetVelocity();
	nextPos = pos + (velocity * deltaTs);
}
