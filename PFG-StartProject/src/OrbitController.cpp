#include "OrbitController.h"
#include "DynamicObject.h"
#include "Scene.h"
#include <iostream>

void OrbitController::Update(float deltaTs, Input* input)
{
	if (input->cmd_x && !start) 
	{
		rk4Orbiter->SetSimulated(true);
		rk2Orbiter->SetSimulated(true);
		eulerOrbiter->SetSimulated(true);

		//Set start velocities
		rk4Orbiter->SetVelocity(20.0f * glm::normalize( glm::cross(glm::vec3(0,1,0), (rk4Centre - rk4Orbiter->GetPosition()) ) ) );
		rk2Orbiter->SetVelocity(20.0f * glm::normalize(glm::cross(glm::vec3(0, 1, 0), (rk2Centre - rk2Orbiter->GetPosition()))));;
		eulerOrbiter->SetVelocity(20.0f * glm::normalize(glm::cross(glm::vec3(0, 1, 0), (eulerCentre - eulerOrbiter->GetPosition()))));;

		start = true;
	}


	if (start) 
	{
		//Counteract gravity
		rk4Orbiter->AddForce(glm::vec3(0, rk4Orbiter->GetMass() * 9.81, 0));
		rk2Orbiter->AddForce(glm::vec3(0, rk2Orbiter->GetMass() * 9.81, 0));
		eulerOrbiter->AddForce(glm::vec3(0, eulerOrbiter->GetMass() * 9.81, 0));

		float rk4Speed = glm::length(rk4Orbiter->GetVelocity());
		float rk2Speed = glm::length(rk2Orbiter->GetVelocity());
		float eulerSpeed = glm::length(eulerOrbiter->GetVelocity());

		//Add cetripetal force (F = mv^2/r)
		rk4Orbiter->AddForce(
			(rk4Orbiter->GetMass() * rk4Speed * rk4Speed) * glm::normalize(rk4Centre - rk4Orbiter->GetPosition())
			/ rk4Radius
		);
		rk2Orbiter->AddForce(
			(rk2Orbiter->GetMass() * rk2Speed * rk2Speed) * glm::normalize(rk2Centre - rk2Orbiter->GetPosition())
			/ rk2Radius
		);
		eulerOrbiter->AddForce(
			(eulerOrbiter->GetMass() * eulerSpeed * eulerSpeed) * glm::normalize(eulerCentre - eulerOrbiter->GetPosition())
			/ eulerRadius
		);

		std::cout << glm::distance(rk4Orbiter->GetPosition(), rk4Centre) << std::endl;
	}

}

void OrbitController::Initialize()
{
	//RK4 object ID = 104
	rk4Orbiter = std::static_pointer_cast<DynamicObject>(scene->FindObjectByID(104));
	rk4Orbiter->integrationMethod = IntegrationMethod::RK4;
	//RK2 object ID = 102
	rk2Orbiter = static_pointer_cast<DynamicObject>(scene->FindObjectByID(102));
	rk2Orbiter->integrationMethod = IntegrationMethod::ExplicitEuler;
	//Euler object ID = 100
	eulerOrbiter = static_pointer_cast<DynamicObject>(scene->FindObjectByID(100));
	eulerOrbiter->integrationMethod = IntegrationMethod::ImplicitEuler;

	//Centre ID = object ID + 1
	rk4Centre = scene->FindObjectByID(105)->GetPosition();
	rk2Centre = scene->FindObjectByID(103)->GetPosition();
	eulerCentre = scene->FindObjectByID(101)->GetPosition();

	//Initialize radii
	rk4Radius = glm::distance(rk4Centre, rk4Orbiter->GetPosition());
	rk2Radius = glm::distance(rk2Centre, rk2Orbiter->GetPosition());
	eulerRadius = glm::distance(eulerCentre, eulerOrbiter->GetPosition());

	start = false;
}
