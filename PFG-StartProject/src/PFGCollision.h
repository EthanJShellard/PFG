#pragma once
#include "glm/glm.hpp"
#include <memory>

class Collider;
class GameObject;

struct Collision
{
	float otherInverseMass;
	float otherBounciness;
	glm::vec3 otherVelocity;
	glm::vec3 collisionPoint;
	glm::vec3 aNormal;
	glm::vec3 otherNormal;

	Collision();
};

namespace PFG
{

	Collision CheckCollision(std::shared_ptr<Collider> A, std::shared_ptr<Collider> B, bool& didCollide);

	float DistanceToPlane(glm::vec3 n, glm::vec3 p, glm::vec3 q);

	bool MovingSphereToPlaneCollision(glm::vec3 planeNormal, glm::vec3 centreBefore, glm::vec3 centreAfter, glm::vec3 pointOnPlane, float radius, glm::vec3& centreAtCollision);

	bool MovingSphereToPlaneCollision(glm::vec3 planeNormal, glm::vec3 centreBefore, glm::vec3 centreAfter, glm::vec3 pointOnPlane, float radius, Collision& collision);

	bool SphereToSphereCollision(glm::vec3 centre0, glm::vec3 centre1, float radius0, float radius1, glm::vec3& contactPoint);

	bool SphereToSphereCollision(glm::vec3 centre0, glm::vec3 centre1, float radius0, float radius1, Collision& collision);

}
