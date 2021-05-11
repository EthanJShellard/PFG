#pragma once
#include "GameObject.h"

class DynamicObject : public GameObject 
{
private:
	//mass in kg
	float mass;
	//Net force on the object
	glm::vec3 netForce;
	//Rate of change of rotation
	glm::vec3 angularVelocity;
	//Sum of torques on this object
	glm::vec3 netTorque;
	//Angular momentum
	glm::vec3 angularMomentum;



public:

	void UpdateModelMatrix();

	void Update(float deltaTs);
	void AddForce(glm::vec3 force);
	void AddTorque(glm::vec3 torque);
	void SetAngularVelocity(glm::vec3 angVel);
	void SetForce(glm::vec3 newForce);
	void ClearForces();
	/// <summary>
	/// Euler numeric integration method
	/// </summary>
	/// <param name="deltaT">Period of time</param>
	void Euler(float deltaTs);
	/// <summary>
	/// Runge Kutta 2 numeric integration method
	/// </summary>
	/// <param name="deltaTs">Period of time</param>
	void RungeKutta2(float deltaTs);
	void RungeKutta4(float deltaTs);

	float GetInverseMass() override;

	float GetMass() override;
	void SetMass(float newMass);

	DynamicObject();
	~DynamicObject();
};