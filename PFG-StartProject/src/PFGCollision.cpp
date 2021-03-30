#include "glm/glm.hpp"
#include "PFGCollision.h"

namespace PFG 
{
	/// <summary>
	/// Distance of point from plane.
	/// </summary>
	/// <param name="n">Normal of plane</param>
	/// <param name="p">Point to find distance to</param>
	/// <param name="q">Any point on plane</param>
	/// <returns>Distance to object from plane</returns>
	float DistanceToPlane(glm::vec3 n, glm::vec3 p, glm::vec3 q) 
	{
		return glm::dot((p-q), glm::normalize(n));
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
			collision.bNormal = planeNormal;
			return true;
		}
		else if (distanceBefore > radius && distanceAfter < radius)
		{
			t = (distanceBefore - radius) / (distanceBefore - distanceAfter);
			collision.collisionPoint = (1 - t) * centreBefore + t * centreAfter;
			collision.bNormal = planeNormal;
			return true;
		}

		return false;
	}
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
		glm::vec3 n = glm::vec3(0);

		if (distance <= (radius0 + radius1)) 
		{
			n = glm::normalize(centre0 - centre1);
			contactPoint = radius0 * n;

			return true;
		}

		return false;
	}

	bool SphereToSphereCollision(glm::vec3 centre0, glm::vec3 centre1, float radius0, float radius1, Collision& collision)
	{
		return false;
	}


}
