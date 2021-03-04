#pragma once
#include "GameObject.h"

class DynamicObject : public GameObject 
{
private:
	//Is the object currently simulated
	bool simulated;

	float radius;
	//mass in kg
	float mass;
	//Velocity in m/s
	glm::vec3 velocity;
	//Net force on the object
	glm::vec3 netForce;
public:

	void UpdateModelMatrix();

	void Update(float deltaTs);
	void AddForce(glm::vec3 force);
	void SetForce(glm::vec3 newForce);
	void ClearForces();
	/// <summary>
	/// Numeric integration method
	/// </summary>
	/// <param name="deltaT">Period of time</param>
	void Euler(float deltaTs);


	void SetMass(float newMass);
	void SetSimulated(bool sim);
	void SetRadius(float newRad);

	DynamicObject();
	~DynamicObject();
};