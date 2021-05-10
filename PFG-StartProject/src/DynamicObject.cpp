#include "DynamicObject.h"
#include "PFGCollision.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <iostream>



void DynamicObject::Update(float deltaTs)
{
	if (simulated)
	{
		//Clear Force
		ClearForces();

		AddForce(glm::vec3(0, mass * -9.81, 0)); //Add Gravity

		//glm::vec3 planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 pointOnPlane = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 nextPosition = position + (velocity * deltaTs);

		//glm::vec3 collisionPosition;

		//bool collision = PFG::MovingSphereToPlaneCollision(planeNormal, position, nextPosition, pointOnPlane, radius, collisionPosition);

		////Fake collision detection
		//if (collision)
		//{
		//	//Impulse collision repsonse demonstration
		//	glm::vec3 planeVelocity = glm::vec3(0,0,0);
		//	glm::vec3 relativeVel = velocity - planeVelocity;
		//	glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);
		//	float surfaceCharacteristics = 0.70f;
		//	//Ja = -(1 + e)(Va- Vb) . n / (1/ma) + (1/mb)
		//	float eCoef = -(1.0f + surfaceCharacteristics) * glm::dot(relativeVel, n);
		//	float invMass = 1 / mass;
		//	float jLin = eCoef / (invMass + 0.0f); //0.0f because floor is static (infinite mass)
		//	glm::vec3 collisionImpulseForce = jLin * n / deltaTs;
		//	AddForce(collisionImpulseForce);

		//	//AddForce(glm::vec3(0.0f, mass * 9.81f, 0.0f)); //Normal Force
		//	//AddForce(glm::vec3(0.0f, -velocity.y * 40.0f, 0.0f)); //Faked Bounce
		//	position.y = radius;
		//	//velocity = glm::vec3(0.0, 0.0, 0.0);
		//}

		glm::vec3 finalReturnPos = position + (velocity * deltaTs);
		float shortestLength = length(finalReturnPos);

		if (collisions.size() > 0) 
		{
			//Impulse collision response for each collision
			for (int i = 0; i < collisions.size(); i++) 
			{
				Collision c = collisions.at(i);
				
				//Go back to return position
				if (float l = glm::length(c.returnPosition - position) < shortestLength)
				{
					position = c.returnPosition;
					shortestLength = l;
					collider->ComputeCentreOfMass();
				}

				//Impulse response
				//	//Ja = -(1 + e)(Va- Vb) . n / (1/ma) + (1/mb				
				glm::vec3 relativeVelocity = velocity - c.otherVelocity;
				float surfaceCharacteristics = collider->bounciness + c.otherBounciness;
				float eCoef = -(1.0f + surfaceCharacteristics) * glm::dot(relativeVelocity, c.otherNormal);
				float invMass = 1 / mass;
				float jLin = eCoef / (invMass + c.otherInverseMass);
				glm::vec3 collisionImpulseForce = jLin * c.otherNormal / deltaTs;
				AddForce(collisionImpulseForce);

				simulated = false;
			}
			
		}


		RungeKutta4(deltaTs);
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

void DynamicObject::RungeKutta2(float deltaTs) 
{
	float oneOverMass = 1.0f / mass;
	
	glm::vec3 force;
	glm::vec3 acceleration;
	//K0 from beginning, k1 from half time
	glm::vec3 k0, k1;

	//evaluate once at t0
	force = netForce;
	acceleration = force / mass;
	k0 = acceleration * deltaTs;

	//evaluate once at half deltaTs
	force = netForce + (k0/2.0f);
	acceleration = force / mass;
	k1 = acceleration * deltaTs;

	//Update velocity using k1
	velocity += k1;

	//Update position using velocity
	position += velocity * deltaTs;
}

void DynamicObject::RungeKutta4(float deltaTs)
{
	float oneOverMass = 1.0f / mass;

	glm::vec3 force;
	glm::vec3 acceleration;
	//K0 from beginning, k1 and k2 at middle, k3 from k2
	glm::vec3 k0, k1, k2, k3;

	//evaluate once at t0
	force = netForce;
	acceleration = force / mass;
	k0 = acceleration * deltaTs;

	//evaluate once at half deltaTs
	force = netForce + (k0 / 2.0f);
	acceleration = force / mass;
	k1 = acceleration * deltaTs;

	//evaluate again at half deltaTs;
	force = netForce + (k1 / 2.0f);
	acceleration = force / mass;
	k2 = acceleration * deltaTs;

	//Evaluate once at full deltaTs
	force = netForce + k2;
	acceleration = force / mass;
	k3 = acceleration * deltaTs;


	//Update velocity using weighted average
	velocity += (k0 + (2.0f * k1)  + (2.0f * k2) + k3) / 6.0f;

	//Update position using velocity
	position += velocity * deltaTs;
}

float DynamicObject::GetInverseMass()
{
	if (mass != 0) return 1 / mass;
	else return 0; //Will make it clear something is wrong
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
	mass = 0;
	netForce = glm::vec3(0);
	angularMomentum = glm::mat4();
	netTorque = glm::quat();
	angularVelocity = glm::quat();
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
	netForce = newForce;
}

void DynamicObject::ClearForces()
{
	netForce = glm::vec3(0);
}

float DynamicObject::GetMass()
{
	return mass;
}

void DynamicObject::SetMass(float newMass)
{
	mass = newMass;
}

void DynamicObject::UpdateModelMatrix()
{
	modelMatrix = glm::mat4();
	modelMatrix = glm::toMat4(rotation) * modelMatrix;
	modelMatrix = glm::translate(glm::mat4(), position) * modelMatrix;
	modelMatrix = glm::scale(modelMatrix, scale);
}