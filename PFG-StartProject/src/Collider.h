#pragma once
#include "glm/glm.hpp"
#include "GameObject.h"
#include <memory>

enum ColliderType { SPHERE, INFINITE_PLANE };


class Collider 
{
public:
	glm::vec3 pos;
	glm::vec3 scale;

	std::shared_ptr<GameObject> parent;
	ColliderType type;
};

class SphereCollider : public Collider
{
	float centre;
	float radius;
};

class InfinitePlaneCollider : public Collider
{
	glm::vec3 normal;
	glm::vec3 point;
};