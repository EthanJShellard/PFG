#pragma once
#include "Collider.h"
#include <memory>

struct Collision 
{
	std::shared_ptr<Collider> colliderA;
	std::shared_ptr<Collider> colliderB;
};