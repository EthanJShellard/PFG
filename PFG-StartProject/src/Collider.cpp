#include "Collider.h"


ColliderType Collider::GetType()
{
	return m_type;
}

glm::mat3 Collider::ComputeInverseInertiaTensor(glm::quat _rotation)
{
	glm::mat3 R = glm::toMat3(_rotation);
	return R * m_inverseBodyInertiaTensor * glm::transpose(R);
}

Collider::Collider()
{
	m_pos = glm::vec3(0);
	m_nextPos = glm::vec3(0);
	m_bounciness = 0;
	m_friction = 0;
	m_parent = nullptr;
	m_mass = 0;
	m_type = ColliderType::SPHERE;
}

SphereCollider::SphereCollider(float _radius)
{
	m_radius = _radius;

	m_type = ColliderType::SPHERE;
}

void SphereCollider::ComputeCentreOfMass()
{
	m_centreOfMass = m_pos;
}

void SphereCollider::Initialise()
{
	m_mass = m_parent->GetMass();

	//COMPUTE INERTIA TENSORS
	//Solid Sphere
	m_bodyInteriaTensor = glm::mat3(
		(2.0f / 5.0f) * m_mass * m_radius * m_radius, 0.0f, 0.0f,
		0.0f, (2.0f / 5.0f) * m_mass * m_radius * m_radius, 0.0f,
		0.0f, 0.0f, (2.0f / 5.0f) * m_mass * m_radius * m_radius);
	//Inverse
	m_inverseBodyInertiaTensor = glm::inverse(m_bodyInteriaTensor);
}

void SphereCollider::Update(float _deltaTs)
{
	m_pos = m_parent->GetPosition();
	m_velocity = m_parent->GetVelocity();
	m_nextPos = m_pos + (m_velocity * _deltaTs);
}

InfinitePlaneCollider::InfinitePlaneCollider(glm::vec3 _planeNormal, glm::vec3 _pointOnPlane)
{
	m_type = ColliderType::INFINITE_PLANE;
	m_planeNormal = _planeNormal;
	m_pointOnPlane = _pointOnPlane;
}

void InfinitePlaneCollider::ComputeCentreOfMass()
{
	m_centreOfMass = m_pos;
}

void InfinitePlaneCollider::Initialise()
{
	m_mass = m_parent->GetMass();
	m_bodyInteriaTensor = glm::mat3();
	m_inverseBodyInertiaTensor = glm::mat3();
}

void InfinitePlaneCollider::Update(float _deltaTs)
{
	m_pos = m_parent->GetPosition();
	m_velocity = m_parent->GetVelocity();
	m_nextPos = m_pos + (m_velocity * _deltaTs);
}
