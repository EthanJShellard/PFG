#include "glm/glm.hpp"
#include "PFGCollision.h"
#include "Collider.h"
#include <iostream>

namespace PFG
{
	glm::vec3 Interpolate(float t, glm::vec3 pos1, glm::vec3 pos2) 
	{
		return ((1 - t) * pos1)  +  (t * pos2);
	}

	/// <summary>
	/// Distance of point from plane.
	/// </summary>
	/// <param name="n">Normal of plane</param>
	/// <param name="p">Point to find distance to</param>
	/// <param name="q">Any point on plane</param>
	/// <returns>Distance to object from plane</returns>
	float DistanceToPlane(glm::vec3 n, glm::vec3 p, glm::vec3 q)
	{
		return glm::dot((p - q), glm::normalize(n));
	}
	/// <summary>
	/// Checks for collision between a moving sphere and a plane. Gives the centre of the sphere at the time of collision
	/// </summary>
	/// <param name="planeNormal">Normal of the plane</param>
	/// <param name="centre0">Centre of sphere at first timestep</param>
	/// <param name="centre1">Centre of sphere at second timestep</param>
	/// <param name="pointOnPlane">A point on the plane</param>
	/// <param name="radius">Radius of the sphere</param>
	/// <param name="centreAtCollision">The centre of the sphere at the time of collision</param>
	/// <returns>True if sphere collides with plane, False otherwise.</returns>
	bool MovingSphereToPlaneCollision(glm::vec3 planeNormal, glm::vec3 centreBefore, glm::vec3 centreAfter, glm::vec3 pointOnPlane, float radius, glm::vec3& centreAtCollision)
	{
		float t;

		float distanceBefore = DistanceToPlane(planeNormal, centreBefore, pointOnPlane);
		float distanceAfter = DistanceToPlane(planeNormal, centreAfter, pointOnPlane);

		if (glm::abs(distanceBefore) <= radius)
		{
			centreAtCollision = centreBefore;
			return true;
		}
		else if (distanceBefore > radius && distanceAfter < radius)
		{
			t = (distanceBefore - radius) / (distanceBefore - distanceAfter);
			centreAtCollision = (1 - t) * centreBefore + t * centreAfter;

			return true;
		}

		return false;
	}
	bool MovingSphereToPlaneCollision(glm::vec3 planeNormal, glm::vec3 centreBefore, glm::vec3 centreAfter, glm::vec3 pointOnPlane, float radius, Collision& collision)
	{
		float t;

		float distanceBefore = DistanceToPlane(planeNormal, centreBefore, pointOnPlane);
		float distanceAfter = DistanceToPlane(planeNormal, centreAfter, pointOnPlane);

		if (glm::abs(distanceBefore) <= radius)
		{
			collision.collisionPoint = centreBefore;
			collision.otherNormal = planeNormal;
			return true;
		}
		else if (distanceBefore > radius && distanceAfter < radius)
		{
			t = (distanceBefore - radius) / (distanceBefore - distanceAfter);
			collision.collisionPoint = (1 - t) * centreBefore + t * centreAfter;
			collision.otherNormal = planeNormal;
			return true;
		}

		return false;
	}
	bool MovingSphereToPlaneCollision(std::shared_ptr<SphereCollider> sph, std::shared_ptr<InfinitePlaneCollider> pla, Collision& collision)
	{
		float distanceBefore = DistanceToPlane(pla->planeNormal, sph->pos, pla->pointOnPlane);
		float distanceAfter = DistanceToPlane(pla->planeNormal, sph->nextPos, pla->pointOnPlane);

		if (glm::abs(distanceBefore) <= sph->radius)
		{
			collision.collisionPoint = sph->pos;
			collision.otherNormal = pla->planeNormal;
			collision.returnPosition = sph->pos + (pla->planeNormal * (sph->radius - distanceBefore));//FIND CORRECT SOLUTION
			collision.otherBounciness = pla->bounciness;
			collision.otherInverseMass = 0;
			return true;
		}
		else if (distanceBefore > sph->radius && distanceAfter <= sph->radius)
		{
			float t;
			t = (distanceBefore - sph->radius) / (distanceBefore - distanceAfter);

			t= std::max(std::min(t, 1.0f), 0.0f); //Clamp to [0,1]
			collision.collisionPoint = Interpolate(t, sph->pos, sph->nextPos);
			collision.otherNormal = pla->planeNormal;
			collision.returnPosition = collision.collisionPoint + (pla->planeNormal * (sph->radius - DistanceToPlane(pla->planeNormal, collision.collisionPoint, pla->pointOnPlane)));//FIND CORRECT SOLUTION
			collision.otherBounciness = pla->bounciness;
			collision.otherInverseMass = 0;
			return true;
		}

		return false;
	}

	bool SphereToSphereCollision(std::shared_ptr<SphereCollider> A, std::shared_ptr<SphereCollider> B, Collision& collision) 
	{
		float distanceBefore = glm::distance( A->pos, B->pos);
		float distanceAfter = glm::distance(A->nextPos, B->nextPos);

		if (distanceBefore <= A->radius + B->radius) 
		{
			collision.otherNormal = glm::normalize(A->pos - B->pos);
			collision.collisionPoint = A->pos + (collision.otherNormal * A->radius);
			collision.returnPosition = A->pos + (collision.otherNormal * (A->radius + B->radius - distanceBefore));// A->pos;//FIND CORRECT SOLUTION
			collision.otherBounciness = B->bounciness;
			collision.otherInverseMass = B->parent->GetInverseMass();
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

	/// <summary>
	/// Checks for collision between two spheres.
	/// </summary>
	/// <param name="centre0">Centre of the first sphere</param>
	/// <param name="centre1">Centre of the second sphere</param>
	/// <param name="radius0">Radius of the first sphere</param>
	/// <param name="radius1">Radius of the second sphere</param>
	/// <param name="contactPoint">The point of contact if a collision took place</param>
	/// <returns>true if collision occured, false otherwise</returns>
	bool SphereToSphereCollision(glm::vec3 centre0, glm::vec3 centre1, float radius0, float radius1, glm::vec3& contactPoint)
	{
		float distance = glm::length(centre0 - centre1);

		if (distance <= (radius0 + radius1))
		{
			glm::vec3 n = glm::vec3(0);
			n = glm::normalize(centre0 - centre1);
			contactPoint = radius0 * n;

			return true;
		}

		return false;
	}


	Collision CheckCollision(std::shared_ptr<Collider> A, std::shared_ptr<Collider> B, bool& didCollide)
	{
		//Perform bounding box check
		//Return if bounding boxes do not intersect, set didCollide to false

		//Determine collider types and call relevant collision check
		Collision c;

		if (A->GetType() == ColliderType::SPHERE)
		{
			if (B->GetType() == ColliderType::INFINITE_PLANE)
			{
				//Assumes plane is static
				didCollide = MovingSphereToPlaneCollision(std::static_pointer_cast<SphereCollider>(A), std::static_pointer_cast<InfinitePlaneCollider>(B), c);
			}
			else if (B->GetType() == ColliderType::SPHERE) 
			{
				didCollide = SphereToSphereCollision(std::static_pointer_cast<SphereCollider>(A), std::static_pointer_cast<SphereCollider>(B), c);
			}
		}

		c.otherVelocity = B->velocity;

		return c;
	}

}

Collision::Collision()
{
	otherInverseMass = 0;
	otherBounciness = 0;
	otherVelocity = glm::vec3(0);
	collisionPoint = glm::vec3(0);
	returnPosition = glm::vec3(0);
	aNormal = glm::vec3(0);
	otherNormal = glm::vec3(0);
}
