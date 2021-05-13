#pragma once
#include "glm/glm.hpp"
#include <memory>

class Collider;
class SphereCollider;
class InfinitePlaneCollider;
class GameObject;

struct Collision
{
	float otherInverseMass;
	float otherBounciness;
	float otherFriction;
	glm::vec3 otherVelocity;
	glm::vec3 collisionPoint;
	glm::vec3 returnPosition;
	glm::vec3 aNormal;
	glm::vec3 otherNormal;

	Collision();
};

namespace PFG
{

	Collision CheckCollision(std::shared_ptr<Collider> A, std::shared_ptr<Collider> B, bool& didCollide);

	float DistanceToPlane(glm::vec3 n, glm::vec3 p, glm::vec3 q);

	bool MovingSphereToPlaneCollision(std::shared_ptr<SphereCollider> sph, std::shared_ptr<InfinitePlaneCollider> pla, Collision& collision);

	bool SphereToSphereCollision(glm::vec3 centre0, glm::vec3 centre1, float radius0, float radius1, glm::vec3& contactPoint);

	bool SphereToSphereCollision(std::shared_ptr<SphereCollider> A, std::shared_ptr<SphereCollider> B, Collision& collision);
}
