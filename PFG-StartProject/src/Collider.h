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
	glm::vec3 scale;
	float bounciness;

	GameObject * parent;

	ColliderType GetType();
	Collider();

protected:
	ColliderType type;
};

struct SphereCollider : public Collider
{
	float radius;
	SphereCollider();
};

struct InfinitePlaneCollider : public Collider
{
	glm::vec3 planeNormal;
	glm::vec3 pointOnPlane;
	InfinitePlaneCollider();
};