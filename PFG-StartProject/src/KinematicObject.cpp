#include "KinematicObject.h"

void KinematicObject::UpdateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(1), position);
	modelMatrix = glm::scale(modelMatrix, scale);
	invModelMatrix = glm::inverse(modelMatrix);
}

void KinematicObject::Update(float deltaTs)
{
	if (simulated) 
	{
		glm::vec3 g = glm::vec3(0.0, -0.0981, 0.0);

		//v = u + at, simulate gravity
		velocity += g * deltaTs;
		position += velocity;

		//Fake collision detection
		if (position.y <= radius)
		{
			position.y = radius;
			velocity = glm::vec3(0.0, 0.0, 0.0);
		}

	}
	UpdateModelMatrix();
}

void KinematicObject::SetRadius(float newRad)
{
	radius = newRad;
}

void KinematicObject::SetSimulated(bool sim)
{
	simulated = sim;
}

KinematicObject::KinematicObject()
{
	// Initialise everything here
	mesh = NULL;
	material = NULL;
	// Set default value
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	simulated = false;
	radius = 1.0f;
}

KinematicObject::~KinematicObject()
{
}
