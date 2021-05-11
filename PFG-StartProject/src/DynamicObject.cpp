#include "DynamicObject.h"
#include "PFGCollision.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <iostream>



void DynamicObject::Update(float deltaTs)
{
	if (simulated)
	{
		glm::vec3 previousNetForce = netForce;

		//Clear Force
		ClearForces();

		AddForce(glm::vec3(0, mass * -9.81, 0)); //Add Gravity

		/*glm::vec3 planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);
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
			float surfaceCharacteristics = 0.70f;
			//Ja = -(1 + e)(Va- Vb) . n / (1/ma) + (1/mb)
			float eCoef = -(1.0f + surfaceCharacteristics) * glm::dot(relativeVel, n);
			float invMass = 1 / mass;
			float jLin = eCoef / (invMass + 0.0f); //0.0f because floor is static (infinite mass)
			glm::vec3 collisionImpulseForce = jLin * n / deltaTs;
			AddForce(collisionImpulseForce);

			//AddForce(glm::vec3(0.0f, mass * 9.81f, 0.0f)); //Normal Force
			//AddForce(glm::vec3(0.0f, -velocity.y * 40.0f, 0.0f)); //Faked Bounce
			position.y = radius;
			//velocity = glm::vec3(0.0, 0.0, 0.0);
		}*/

		glm::vec3 finalReturnPos = position + (velocity * deltaTs);
		float shortestLength = length(finalReturnPos - position);

		if (collisions.size() > 0) 
		{
			//Collision response for each collision
			for (int i = 0; i < collisions.size(); i++) 
			{
				Collision c = collisions.at(i);
				
				//Compute return position
				if (float l = glm::length(c.returnPosition - position) < shortestLength)
				{
					position = c.returnPosition;
					shortestLength = l;
					collider->ComputeCentreOfMass();
				}

				//Normal force
				glm::vec3 normalForce = c.otherNormal * 0.5f * glm::dot(previousNetForce, -c.otherNormal);
				AddForce(normalForce);

				//Linear impulse response
				//	//Ja = -(1 + e)(Va- Vb) . n / (1/ma) + (1/mb				
				glm::vec3 relativeVelocity = velocity - c.otherVelocity;
				float surfaceCharacteristics = collider->bounciness + c.otherBounciness;
				float eCoef = -(1.0f + surfaceCharacteristics) * glm::dot(relativeVelocity, c.otherNormal);
				float invMass = 1 / mass;
				float jLin = eCoef / (invMass + c.otherInverseMass);
				glm::vec3 collisionImpulseForce = jLin * c.otherNormal / deltaTs;
				AddForce(collisionImpulseForce);

				//Angular response
				//Impulse Torque = (contact_point - COM) cross Jlin
				glm::vec3 r1 = collider->centreOfMass - c.collisionPoint;
				glm::vec3 angularImpulse = glm::cross(r1, jLin * c.otherNormal);
				AddTorque(angularImpulse);
				///////////////////

				//Compute friction
				//Find relative velocity perpendicular to contact normal
				glm::vec3 forwardRelativeVelocity = relativeVelocity - glm::dot(relativeVelocity, c.otherNormal) * c.otherNormal;
				//Get a normalized vector of the direction travelled perpendicular to the contact normal
				glm::vec3 forwardRelativeDirection;
				if (glm::length(forwardRelativeVelocity) != 0.0f) 
				{
					forwardRelativeDirection = glm::normalize(forwardRelativeVelocity);
				}
				else 
				{
					forwardRelativeDirection = glm::vec3(0.0f,0.0f,0.0f);
				}
				//Friction acts opposite to velocity
				float mu = c.otherFriction + collider->friction;
				glm::vec3 frictionDirection = -forwardRelativeDirection;
				//Friction = mu * normal force
				glm::vec3 frictionForce = mu * frictionDirection * glm::length(normalForce);
				AddForce(frictionForce);
				//////////////////////

				//Compute Torque
				glm::vec3 tempTorque = (glm::cross(r1, normalForce) + glm::cross(r1, frictionForce));
				//Damping
				tempTorque -= angularMomentum * 20.0f;
				AddTorque(tempTorque);
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

	//Compute Angular Momentum
	angularMomentum += netTorque * deltaTs;
	
	glm::mat3 inverseInertiaTensor = collider->ComputeInverseInertiaTensor(rotation);

	//Compute angular velocity
	angularVelocity = inverseInertiaTensor * angularMomentum;

	//Compute skew matrix omega star
	glm::mat3 omega_star = glm::mat3(0.0f, -angularVelocity.z, angularVelocity.y,
		angularVelocity.z, 0.0f, -angularVelocity.x,
		-angularVelocity.y, angularVelocity.x, 0.0f);

	glm::mat3 R = glm::toMat3(rotation);

	//Update orientation
	R += omega_star * R * deltaTs;
	rotation = R;
	rotation = glm::normalize(rotation);
}

void DynamicObject::RungeKutta2(float deltaTs) 
{
	float oneOverMass = 1.0f / mass;

	glm::vec3 force;
	glm::vec3 acceleration;
	//K0 from beginning, k1 from half time
	glm::vec3 k0, k1;

	//LINEAR

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


	//ANGULAR
	//Evaluate once at t0
	glm::vec3 torque;
	torque = netTorque;
	k0 = torque * deltaTs;

	//Evaluate once at half deltaTs
	torque = netTorque + k0 / 2.0f;
	k1 = torque * deltaTs;
	//Update angular momentum
	angularMomentum += k1;

	glm::mat3 inverseInertiaTensor = collider->ComputeInverseInertiaTensor(rotation);

	//Compute angular velocity
	angularVelocity = inverseInertiaTensor * angularMomentum;

	//Compute skew matrix omega star
	glm::mat3 omega_star = glm::mat3(0.0f, -angularVelocity.z, angularVelocity.y,
		angularVelocity.z, 0.0f, -angularVelocity.x,
		-angularVelocity.y, angularVelocity.x, 0.0f);

	glm::mat3 R = glm::toMat3(rotation);

	//Update orientation
	R += omega_star * R * deltaTs;
	rotation = R;
	rotation = glm::normalize(rotation);
}

void DynamicObject::RungeKutta4(float deltaTs)
{
	float oneOverMass = 1.0f / mass;

	glm::vec3 force;
	glm::vec3 acceleration;
	//K0 from beginning, k1 and k2 at middle, k3 from k2
	glm::vec3 k0, k1, k2, k3;

	//LINEAR

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

	//ANGULAR
	//Evaluate once at t0
	glm::vec3 torque;
	torque = netTorque;
	k0 = torque * deltaTs;

	//Evaluate once at half deltaTs
	torque = netTorque + k0 / 2.0f;
	k1 = torque * deltaTs;

	//Evaluate again at half deltaTs
	torque = netTorque + k1 / 2.0f;
	k2 = torque * deltaTs;

	//Evaluate at deltaTs
	torque = netTorque + k2;
	k3 = torque * deltaTs;

	//Update angular momentum
	angularMomentum += (k0 + (2.0f * k1) + (2.0f * k2) + k3) / 6.0f;

	glm::mat3 inverseInertiaTensor = collider->ComputeInverseInertiaTensor(rotation);

	//Compute angular velocity
	angularVelocity = inverseInertiaTensor * angularMomentum;

	//Compute skew matrix omega star
	glm::mat3 omega_star = glm::mat3(0.0f, -angularVelocity.z, angularVelocity.y,
		angularVelocity.z, 0.0f, -angularVelocity.x,
		-angularVelocity.y, angularVelocity.x, 0.0f);

	glm::mat3 R = glm::toMat3(rotation);

	//Update orientation
	R += omega_star * R * deltaTs;
	rotation = R;
	rotation = glm::normalize(rotation);
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
	angularMomentum = glm::vec3();
	netTorque = glm::vec3();
	angularVelocity = glm::vec3();
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::AddForce(glm::vec3 force)
{
	netForce += force;
}

void DynamicObject::AddTorque(glm::vec3 torque)
{
	netTorque += torque;
}

void DynamicObject::SetForce(glm::vec3 newForce)
{
	netForce = newForce;
}

void DynamicObject::ClearForces()
{
	netForce = glm::vec3(0);
	netTorque = glm::vec3();
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