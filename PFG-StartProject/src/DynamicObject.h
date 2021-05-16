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
	//Implicit Euler integration
	void ImplicitEuler(float _deltaTs);
	//Explicit Euler integration
	void ExplicitEuler(float _deltaTs);
	//Velocity Verlet integration
	void VelocityVerlet(float _deltaTs);

	//Safely get inverse mass. Will return 0 if mass is 0.
	float GetInverseMass() override;

	float GetMass() override;
	void SetMass(float _newMass);

	DynamicObject();
	~DynamicObject();

	IntegrationMethod m_integrationMethod;
};