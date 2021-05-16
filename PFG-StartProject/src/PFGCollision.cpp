#include "glm/glm.hpp"
#include "PFGCollision.h"
#include "Collider.h"
#include "GameObject.h"
#include <iostream>

namespace PFG
{
	//Linear interpolation between 2 vec 3s
	glm::vec3 Interpolate(float _t, glm::vec3 _pos1, glm::vec3 _pos2)
	{
		return ((1 - _t) * _pos1) + (_t * _pos2);
	}

	float DistanceToPlane(glm::vec3 _n, glm::vec3 _p, glm::vec3 _q)
	{
		return glm::dot((_p - _q), glm::normalize(_n));
	}

	bool MovingSphereToPlaneCollision(std::shared_ptr<SphereCollider> _sph, std::shared_ptr<InfinitePlaneCollider> _pla, Collision& _collision)
	{
		float distanceBefore = DistanceToPlane(_pla->m_planeNormal, _sph->m_pos, _pla->m_pointOnPlane);
		float distanceAfter = DistanceToPlane(_pla->m_planeNormal, _sph->m_nextPos, _pla->m_pointOnPlane);

		if (glm::abs(distanceBefore) <= _sph->m_radius)
		{
			_collision.m_otherNormal = (glm::dot(_pla->m_planeNormal, _sph->m_pos - _pla->m_pointOnPlane) >= 0) ? _pla->m_planeNormal : -_pla->m_planeNormal;
			_collision.m_collisionPoint = _sph->m_pos - (_pla->m_planeNormal * (distanceBefore));
			_collision.m_returnPosition = _sph->m_pos - (_sph->m_radius * -_collision.m_otherNormal) - (_collision.m_otherNormal * distanceBefore);
			return true;
		}
		else if (distanceBefore > _sph->m_radius && distanceAfter <= _sph->m_radius)
		{

			float t;
			t = (distanceBefore - _sph->m_radius) / (distanceBefore - distanceAfter);
			t = std::max(std::min(t, 1.0f), 0.0f); //Clamp to [0,1]
			glm::vec3 posAtCollision = Interpolate(t, _sph->m_pos, _sph->m_nextPos);
			_collision.m_otherNormal = (glm::dot(_pla->m_planeNormal, _sph->m_pos - _pla->m_pointOnPlane) >= 0) ? _pla->m_planeNormal : -_pla->m_planeNormal;
			_collision.m_collisionPoint = posAtCollision - (_collision.m_otherNormal * (_sph->m_radius));
			_collision.m_returnPosition = posAtCollision;
			return true;
		}

		return false;
	}

	bool SphereToSphereCollision(std::shared_ptr<SphereCollider> _A, std::shared_ptr<SphereCollider> _B, Collision& _collision)
	{
		float distanceBefore = glm::distance(_A->m_pos, _B->m_pos);
		float distanceAfter = glm::distance(_A->m_nextPos, _B->m_nextPos);

		if (distanceBefore <= _A->m_radius + _B->m_radius)
		{
			_collision.m_otherNormal = glm::normalize(_A->m_pos - _B->m_pos);
			_collision.m_collisionPoint = _A->m_pos + (_collision.m_otherNormal * _A->m_radius);
			_collision.m_returnPosition = _A->m_pos + (_collision.m_otherNormal * (_A->m_radius + _B->m_radius - distanceBefore));// A->pos;//FIND CORRECT SOLUTION??

			return true;
		}
		//else if (distanceBefore > A->radius + B->radius && distanceAfter <= A->radius + B->radius)
		//{
		//	float t = (distanceBefore - A->radius) / (distanceBefore - distanceAfter);
		//	t = std::max(std::min(t, 1.0f), 0.0f); //Clamp to [0,1]
		//	std::cout << t << std::endl;
		//	collision.collisionPoint = Interpolate(t, A->pos, A->nextPos);
		//	collision.otherNormal = glm::normalize(A->pos - B->pos);
		//	collision.returnPosition = Interpolate(t, B->pos, B->nextPos) + (collision.otherNormal * ((A->radius + B->radius))); //collision.collisionPoint; //FIND CORRECT SOLUTION
		//	collision.otherBounciness = B->bounciness;
		//	collision.otherInverseMass = B->parent->GetInverseMass();
		//	return true;
		//}

		return false;
	};

	bool SphereToSphereCollision(glm::vec3 _centre0, glm::vec3 _centre1, float _radius0, float _radius1, glm::vec3& _contactPoint)
	{
		float distance = glm::length(_centre0 - _centre1);

		if (distance <= (_radius0 + _radius1))
		{
			glm::vec3 n = glm::vec3(0);
			n = glm::normalize(_centre0 - _centre1);
			_contactPoint = _radius0 * n;

			return true;
		}

		return false;
	}

	Collision CheckCollision(std::shared_ptr<Collider> _A, std::shared_ptr<Collider> _B, bool& _didCollide)
	{
		//Perform bounding box check
		//Return if bounding boxes do not intersect, set didCollide to false

		//Determine collider types and call relevant collision check
		Collision c;

		if (_A->GetType() == ColliderType::SPHERE)
		{
			if (_B->GetType() == ColliderType::INFINITE_PLANE)
			{
				//Assumes plane is static
				_didCollide = MovingSphereToPlaneCollision(std::static_pointer_cast<SphereCollider>(_A), std::static_pointer_cast<InfinitePlaneCollider>(_B), c);
			}
			else if (_B->GetType() == ColliderType::SPHERE)
			{
				_didCollide = SphereToSphereCollision(std::static_pointer_cast<SphereCollider>(_A), std::static_pointer_cast<SphereCollider>(_B), c);
			}
		}

		c.m_otherVelocity = _B->m_velocity;
		c.m_otherBounciness = _B->m_bounciness;
		c.m_otherInverseMass = _B->m_parent->GetInverseMass();
		c.m_otherFriction = _B->m_friction;

		return c;
	}
}

Collision::Collision()
{
	m_otherInverseMass = 0;
	m_otherBounciness = 0;
	m_otherFriction = 0;
	m_otherVelocity = glm::vec3(0);
	m_collisionPoint = glm::vec3(0);
	m_returnPosition = glm::vec3(0);
	m_aNormal = glm::vec3(0);
	m_otherNormal = glm::vec3(0);
}
