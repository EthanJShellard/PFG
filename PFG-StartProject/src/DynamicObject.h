#pragma once
#include "GameObject.h"

enum class IntegrationMethod {IMPLICITEULER, EXPLICITEULER, VELOCITYVERLET};

class DynamicObject : public GameObject 
{
private:
	//mass in kg
	float m_mass;
	//Net force on the object
	glm::vec3 m_netForce;
	//Rate of change of rotation
	glm::vec3 m_angularVelocity;
	//Sum of torques on this object
	glm::vec3 m_netTorque;
	//Angular momentum
	glm::vec3 m_angularMomentum;
	//Acceleration used in velocity verlet integration
	glm::vec3 m_acceleration;

public:

	void UpdateModelMatrix();

	void Update(float _deltaTs);
	void AddForce(glm::vec3 _force);
	void AddTorque(glm::vec3 _torque);
	void SetAngularVelocity(glm::vec3 _angVel);
	void SetForce(glm::vec3 _newForce);
	void ClearForces();
	/// <summary>
	/// Euler numeric integration method
	/// </summary>
	/// <param name="deltaT">Period of time</param>
	void ImplicitEuler(float _deltaTs);
	void ExplicitEuler(float _deltaTs);
	void VelocityVerlet(float _deltaTs);

	float GetInverseMass() override;

	float GetMass() override;
	void SetMass(float _newMass);

	DynamicObject();
	~DynamicObject();

	IntegrationMethod m_integrationMethod;
};