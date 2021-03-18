#pragma once
#include "glm/glm.hpp"

namespace PFG
{
	float DistanceToPlane(glm::vec3 n, glm::vec3 p, glm::vec3 q);

	bool MovingSphereToPlaneCollision(glm::vec3 planeNormal, glm::vec3 centreBefore, glm::vec3 centreAfter, glm::vec3 pointOnPlane, float radius, glm::vec3& centreAtCollision);

	bool SphereToSphereCollision(glm::vec3 centre0, glm::vec3 centre1, float radius0, float radius1, glm::vec3& contactPoint);
}
