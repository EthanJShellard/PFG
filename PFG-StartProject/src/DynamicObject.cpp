#include "DynamicObject.h"
#include "PFGCollision.h"
#include <GLM/gtc/matrix_transform.hpp>



void DynamicObject::Update(float deltaTs)
{
	//Using F=MA, calculate velocity and position

	if (simulated)
	{
		//Clear Force
		ClearForces();

		//Compute Forces on the object
		AddForce(glm::vec3(0, mass * -9.81, 0)); //Add Gravity

		glm::vec3 planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 pointOnPlane = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 nextPosition = position + (velocity * deltaTs);

		glm::vec3 collisionPosition;

		bool collision = PFG::MovingSphereToPlaneCollision(planeNormal, position, nextPosition, pointOnPlane, radius, collisionPosition);

		//Fake collision detection
		if (collision)
		{
			//Impulse collision repsonse demonstration
			glm::vec3 planeVelocity = glm::vec3(0,0,0);
			glm::vec3 relativeVel = velocity - planeVelocity;
			glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);
			float planeMass = FLT_MAX;
			float eCof = -(1.0f + 0.8f) * glm::dot(relativeVel, n);
			float invMass = 1 / mass;
			float jLin = eCof / (invMass + 0.0f); //0.0f because floor is static (infinite mass)

			glm::vec3 collisionImpulseForce = jLin * n / deltaTs;
			AddForce(collisionImpulseForce);

			//AddForce(glm::vec3(0.0f, mass * 9.81f, 0.0f)); //Normal Force
			//AddForce(glm::vec3(0.0f, -velocity.y * 40.0f, 0.0f)); //Faked Bounce
			position.y = radius;
			//velocity = glm::vec3(0.0, 0.0, 0.0);
		}


		Euler(deltaTs);
	}
	UpdateModelMatrix();
}

void DynamicObject::Euler(float deltaTs)
{
	float oneOverMass = 1.0f / mass;
	//update velocity using euler algorithm (a = f/m)
	velocity += (netForce * oneOverMass) * deltaTs;
	//Update position using velocity
	position += velocity * deltaTs;
}

void DynamicObject::SetSimulated(bool sim)
{
	simulated = sim;
}

void DynamicObject::SetRadius(float newRad)
{
	radius = newRad;
}

DynamicObject::DynamicObject()
{
	// Initialise everything here
	mesh = NULL;
	material = NULL;
	// Set default value
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	simulated = false;
	radius = 1.0f;
	mass = 0;
	netForce = glm::vec3(0);
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::AddForce(glm::vec3 force)
{
	netForce += force;
}

void DynamicObject::SetForce(glm::vec3 newForce)
{
	newForce = newForce;
}

void DynamicObject::ClearForces()
{
	netForce = glm::vec3(0);
}

void DynamicObject::SetMass(float newMass)
{
	mass = newMass;
}

void DynamicObject::UpdateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(1), position);
	modelMatrix = glm::scale(modelMatrix, scale);
	invModelMatrix = glm::inverse(modelMatrix);
}