#pragma once
#include "glm/glm.hpp"
#include <memory>

class GameObject;

namespace PFG
{
	struct Collision
	{
		std::shared_ptr<GameObject> a;
		std::shared_ptr<GameObject> b;
		glm::vec3 collisionPoint;
		glm::vec3 aNormal;
		glm::vec3 bNormal;
	};

	float DistanceToPlane(glm::vec3 n, glm::vec3 p, glm::vec3 q);

	bool MovingSphereToPlaneCollision(glm::vec3 planeNormal, glm::vec3 centreBefore, glm::vec3 centreAfter, glm::vec3 pointOnPlane, float radius, glm::vec3& centreAtCollision);

	bool MovingSphereToPlaneCollision(glm::vec3 planeNormal, glm::vec3 centreBefore, glm::vec3 centreAfter, glm::vec3 pointOnPlane, float radius, Collision& collision);

	bool SphereToSphereCollision(glm::vec3 centre0, glm::vec3 centre1, float radius0, float radius1, glm::vec3& contactPoint);

	bool SphereToSphereCollision(glm::vec3 centre0, glm::vec3 centre1, float radius0, float radius1, Collision& collision);

	enum ColliderType {SPHERE, PLANE};

	
}
