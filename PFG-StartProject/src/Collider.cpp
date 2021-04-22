#include "Collider.h"


ColliderType Collider::GetType()
{
	return type;
}

Collider::Collider()
{
	pos = glm::vec3(0);
	nextPos = glm::vec3(0);
	scale = glm::vec3(1);
	bounciness = 0;

	type = ColliderType::SPHERE;
}

SphereCollider::SphereCollider()
{
	radius = 0.0f;
	type = ColliderType::SPHERE;
}

InfinitePlaneCollider::InfinitePlaneCollider()
{
	type = ColliderType::INFINITE_PLANE;
}
