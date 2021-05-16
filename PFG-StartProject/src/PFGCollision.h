#pragma once
#include "glm/glm.hpp"
#include <memory>

class Collider;
class SphereCollider;
class InfinitePlaneCollider;
class GameObject;

struct Collision
{
	float m_otherInverseMass;
	float m_otherBounciness;
	float m_otherFriction;
	glm::vec3 m_otherVelocity;
	glm::vec3 m_collisionPoint;
	glm::vec3 m_returnPosition;
	glm::vec3 m_aNormal;
	glm::vec3 m_otherNormal;

	Collision();
};

namespace PFG
{

	Collision CheckCollision(std::shared_ptr<Collider> _A, std::shared_ptr<Collider> _B, bool& _didCollide);

	float DistanceToPlane(glm::vec3 _n, glm::vec3 _p, glm::vec3 _q);

	bool MovingSphereToPlaneCollision(std::shared_ptr<SphereCollider> _sph, std::shared_ptr<InfinitePlaneCollider> _pla, Collision& _collision);

	bool SphereToSphereCollision(glm::vec3 _centre0, glm::vec3 _centre1, float _radius0, float _radius1, glm::vec3& _contactPoint);

	bool SphereToSphereCollision(std::shared_ptr<SphereCollider> _A, std::shared_ptr<SphereCollider> _B, Collision& _collision);
}
