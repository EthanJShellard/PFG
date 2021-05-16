#include "DynamicObject.h"
#include "PFGCollision.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <iostream>



void DynamicObject::Update(float _deltaTs)
{
	if (m_simulated)
	{
		glm::vec3 previousNetForce = m_netForce;
		glm::vec3 gravity = glm::vec3(0, m_mass * -9.81, 0);


		AddForce(gravity); //Add Gravity

		if (m_collisions.size() > 0) 
		{
			glm::vec3 returnTranslate = glm::vec3(0);

			//Sum return positions for each collision
			for (int i = 0; i < m_collisions.size(); i++)
			{
				//Add return translation
				returnTranslate += (m_collisions.at(i).m_returnPosition - m_position);
			}

			//Handle return positioning
			m_position += returnTranslate;
			//position = collisions.at(0).returnPosition;
			m_collider->ComputeCentreOfMass();

			//Collision response for each collision
			for (int i = 0; i < m_collisions.size(); i++) 
			{
				Collision c = m_collisions.at(i);
				
				

				//Normal force
				glm::vec3 normalForce = c.m_otherNormal * glm::dot(gravity, -c.m_otherNormal); //Query
				AddForce(normalForce);

				//Linear impulse response
				//	//Ja = -(1 + e)(Va- Vb) . n / (1/ma) + (1/mb				
				glm::vec3 relativeVelocity = m_velocity - c.m_otherVelocity;
				float surfaceCharacteristics = std::max(std::min(m_collider->m_bounciness + c.m_otherBounciness, 1.0f), 0.0f);
				float eCoef = -(1.0f + surfaceCharacteristics) * glm::dot(relativeVelocity, c.m_otherNormal);
				float invMass = 1 / m_mass;
				float jLin = eCoef / (invMass + c.m_otherInverseMass);
				glm::vec3 collisionImpulseForce = jLin * c.m_otherNormal / _deltaTs;
				AddForce(collisionImpulseForce);

				//Angular response
				//Impulse Torque = (contact_point - COM) cross Jlin
				glm::vec3 r1 = m_collider->m_centreOfMass - c.m_collisionPoint;
				glm::vec3 angularImpulse = glm::cross(r1, jLin * c.m_otherNormal);
				AddTorque(angularImpulse);
				///////////////////

				//Compute friction
				//Find relative velocity perpendicular to contact normal
				glm::vec3 forwardRelativeVelocity = relativeVelocity - glm::dot(relativeVelocity, c.m_otherNormal) * c.m_otherNormal;
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
				float mu = c.m_otherFriction + m_collider->m_friction;
				glm::vec3 frictionDirection = -forwardRelativeDirection;
				//Friction = mu * normal force
				glm::vec3 frictionForce = mu * frictionDirection * glm::length(normalForce);
				AddForce(frictionForce);
				//////////////////////

				//Compute Torque
				glm::vec3 tempTorque = (glm::cross(r1, normalForce) + glm::cross(r1, frictionForce));
				//Damping
				tempTorque -= m_angularMomentum * 20.0f;
				AddTorque(tempTorque);
			}

		}


		if (m_collider)
		{
			switch (m_integrationMethod) 
			{
			case IntegrationMethod::IMPLICITEULER: ImplicitEuler(_deltaTs); break;
			case IntegrationMethod::EXPLICITEULER: ExplicitEuler(_deltaTs); break;
			case IntegrationMethod::VELOCITYVERLET: VelocityVerlet(_deltaTs); break;
			}
		}

		//Clear Force
		ClearForces();

	}
	UpdateModelMatrix();
}

void DynamicObject::ImplicitEuler(float _deltaTs)
{
	float oneOverMass = GetInverseMass();
	//update velocity using euler algorithm (a = f/m)
	m_velocity += (m_netForce * oneOverMass) * _deltaTs;
	//Update position using velocity
	m_position += m_velocity * _deltaTs;

	//Compute Angular Momentum
	m_angularMomentum += m_netTorque * _deltaTs;
	
	glm::mat3 inverseInertiaTensor = m_collider->ComputeInverseInertiaTensor(m_rotation);

	//Compute angular velocity
	m_angularVelocity = inverseInertiaTensor * m_angularMomentum;

	//Compute skew matrix omega star
	glm::mat3 omega_star = glm::mat3(0.0f, -m_angularVelocity.z, m_angularVelocity.y,
		m_angularVelocity.z, 0.0f, -m_angularVelocity.x,
		-m_angularVelocity.y, m_angularVelocity.x, 0.0f);

	glm::mat3 R = glm::toMat3(m_rotation);

	//Update orientation
	R += omega_star * R * _deltaTs;
	m_rotation = R;
	m_rotation = glm::normalize(m_rotation);
}

void DynamicObject::ExplicitEuler(float _deltaTs)
{
	float oneOverMass = GetInverseMass();

	//Update position using velocity
	m_position += m_velocity * _deltaTs;
	//update velocity using euler algorithm (a = f/m)
	m_velocity += (m_netForce * oneOverMass) * _deltaTs;
	

	glm::mat3 inverseInertiaTensor = m_collider->ComputeInverseInertiaTensor(m_rotation);

	//Compute angular velocity
	m_angularVelocity = inverseInertiaTensor * m_angularMomentum;

	//Compute skew matrix omega star
	glm::mat3 omega_star = glm::mat3(0.0f, -m_angularVelocity.z, m_angularVelocity.y,
		m_angularVelocity.z, 0.0f, -m_angularVelocity.x,
		-m_angularVelocity.y, m_angularVelocity.x, 0.0f);

	glm::mat3 R = glm::toMat3(m_rotation);

	//Update orientation
	R += omega_star * R * _deltaTs;
	m_rotation = R;
	m_rotation = glm::normalize(m_rotation);

	//Compute Angular Momentum
	m_angularMomentum += m_netTorque * _deltaTs;

	
}

//Tested and doesn't seem to be functioning correctly. Acts almost exactly the same as explicit Euler.
//Theoretically should be a velocity verlet implementation though
void DynamicObject::VelocityVerlet(float _deltaTs)
{
	//Update position
	glm::vec3 newPos = m_position + m_velocity * _deltaTs + m_acceleration * (0.5f * _deltaTs * _deltaTs);
	//Update acceleration
	glm::vec3 newAccelaration = m_netForce / m_mass;
	//Update velocity
	glm::vec3 newVel = m_velocity + (m_acceleration + newAccelaration) * (0.5f * _deltaTs);

	m_position = newPos;
	m_velocity = newVel;
	m_acceleration = newAccelaration;
}


float DynamicObject::GetInverseMass()
{
	if (m_mass != 0) return 1 / m_mass;
	else return 0; //Will make it clear something is wrong
}


DynamicObject::DynamicObject()
{
	// Initialise everything here
	m_mesh = NULL;
	m_material = NULL;
	// Set default value
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_simulated = false;
	m_mass = 0;
	m_netForce = glm::vec3(0);
	m_angularMomentum = glm::vec3();
	m_netTorque = glm::vec3();
	m_angularVelocity = glm::vec3();
	m_integrationMethod = IntegrationMethod::IMPLICITEULER;
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::AddForce(glm::vec3 _force)
{
	m_netForce += _force;
}

void DynamicObject::AddTorque(glm::vec3 _torque)
{
	m_netTorque += _torque;
}

void DynamicObject::SetAngularVelocity(glm::vec3 _angVel)
{
	m_angularVelocity = _angVel;
}

void DynamicObject::SetForce(glm::vec3 _newForce)
{
	m_netForce = _newForce;
}

void DynamicObject::ClearForces()
{
	m_netForce = glm::vec3(0);
	m_netTorque = glm::vec3();
}


float DynamicObject::GetMass()
{
	return m_mass;
}

void DynamicObject::SetMass(float _newMass)
{
	m_mass = _newMass;
}

void DynamicObject::UpdateModelMatrix()
{
	m_modelMatrix = glm::mat4();
	m_modelMatrix = glm::toMat4(m_rotation) * m_modelMatrix;
	m_modelMatrix = glm::translate(glm::mat4(), m_position) * m_modelMatrix;
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}