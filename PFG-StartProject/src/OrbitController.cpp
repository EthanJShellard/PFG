#include "OrbitController.h"
#include "DynamicObject.h"
#include "Scene.h"
#include <iostream>
#include <fstream>

void OrbitController::Update(float deltaTs, Input* input)
{
	if (input->cmd_x && !start) 
	{
		explicitOrbiter->SetSimulated(true);
		semiImplicitOrbiter->SetSimulated(true);

		//Set start velocities
		explicitOrbiter->SetVelocity(2.0f * glm::normalize(glm::cross(glm::vec3(0, 1, 0), (explicitCentre - explicitOrbiter->GetPosition()))));;
		semiImplicitOrbiter->SetVelocity(2.0f * glm::normalize(glm::cross(glm::vec3(0, 1, 0), (semiImplicitCentre - semiImplicitOrbiter->GetPosition()))));;

		start = true;
	}


	if (start) 
	{
		//Counteract gravity
		explicitOrbiter->AddForce(glm::vec3(0, explicitOrbiter->GetMass() * 9.81, 0));
		semiImplicitOrbiter->AddForce(glm::vec3(0, semiImplicitOrbiter->GetMass() * 9.81, 0));

		float rk2Speed = glm::length(explicitOrbiter->GetVelocity());
		float eulerSpeed = glm::length(semiImplicitOrbiter->GetVelocity());

		//Add cetripetal force (F = mv^2/r)
		explicitOrbiter->AddForce(
			(explicitOrbiter->GetMass() * rk2Speed * rk2Speed) * glm::normalize(explicitCentre - explicitOrbiter->GetPosition())
			/ explicitRadius
		);
		semiImplicitOrbiter->AddForce(
			(semiImplicitOrbiter->GetMass() * eulerSpeed * eulerSpeed) * glm::normalize(semiImplicitCentre - semiImplicitOrbiter->GetPosition())
			/ semiImplicitRadius
		);

		
		explicitSpeeds.push_back(glm::length(explicitOrbiter->GetVelocity()));
		semiImplicitSpeeds.push_back(glm::length(semiImplicitOrbiter->GetVelocity()));


		if (input->cmd_x && !fileWrote) 
		{
			std::ofstream outputFile("logs/orbitlogs.txt");

			outputFile << "Orbital integration results\nExplicit | Semi-implicit\n";

			for (size_t i = 0; i < explicitSpeeds.size(); i++) 
			{
				outputFile << explicitSpeeds.at(i) << "		 |		 " << semiImplicitSpeeds.at(i) << std::endl;
			}

			outputFile.close();
		}
	}

}

void OrbitController::Initialize()
{
	//RK2 object ID = 102
	explicitOrbiter = static_pointer_cast<DynamicObject>(scene->FindObjectByID(102));
	explicitOrbiter->integrationMethod = IntegrationMethod::EXPLICITEULER;
	//Euler object ID = 100
	semiImplicitOrbiter = static_pointer_cast<DynamicObject>(scene->FindObjectByID(100));
	semiImplicitOrbiter->integrationMethod = IntegrationMethod::IMPLICITEULER;

	//Centre ID = object ID + 1
	explicitCentre = scene->FindObjectByID(103)->GetPosition();
	semiImplicitCentre = scene->FindObjectByID(101)->GetPosition();

	//Initialize radii
	explicitRadius = glm::distance(explicitCentre, explicitOrbiter->GetPosition());
	semiImplicitRadius = glm::distance(semiImplicitCentre, semiImplicitOrbiter->GetPosition());

	start = false;
	fileWrote = false;
}
