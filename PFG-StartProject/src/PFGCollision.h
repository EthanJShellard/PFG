#pragma once
#include "glm/glm.hpp"
#include <memory>

struct Collider;
struct SphereCollider;
struct InfinitePlaneCollider;
class GameObject;

struct Collision
{
	//Information about object collided with
	float m_otherInverseMass;
	float m_otherBounciness;
	float m_otherFriction;
	glm::vec3 m_otherVelocity;
	glm::vec3 m_otherNormal;
	//Information about collision
	glm::vec3 m_collisionPoint;
	glm::vec3 m_returnPosition;
	glm::vec3 m_aNormal;

	Collision();
};

namespace PFG
{
	//Check if 2 colliders collide
	Collision CheckCollision(std::shared_ptr<Collider> _A, std::shared_ptr<Collider> _B, bool& _didCollide);
	//Get distance from point to plane
	float DistanceToPlane(glm::vec3 _n, glm::vec3 _p, glm::vec3 _q);
	//Check if a moving sphere collides with a plane and respond appropriately
	bool MovingSphereToPlaneCollision(std::shared_ptr<SphereCollider> _sph, std::shared_ptr<InfinitePlaneCollider> _pla, Collision& _collision);
	//Check if two moving spheres collide
	bool SphereToSphereCollision(glm::vec3 _centre0, glm::vec3 _centre1, float _radius0, float _radius1, glm::vec3& _contactPoint);
	//Check if two moving spheres collide and respond appropriately
	bool SphereToSphereCollision(std::shared_ptr<SphereCollider> _A, std::shared_ptr<SphereCollider> _B, Collision& _collision);
}
