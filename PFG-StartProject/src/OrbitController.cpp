#include "OrbitController.h"
#include "DynamicObject.h"
#include "Scene.h"
#include <iostream>
#include <fstream>

void OrbitController::Update(float _deltaTs, Input* _input)
{
	if (_input->cmd_x && !m_start) 
	{
		m_explicitOrbiter->SetSimulated(true);
		m_semiImplicitOrbiter->SetSimulated(true);

		//Set start velocities
		m_explicitOrbiter->SetVelocity(2.0f * glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_explicitCentre - m_explicitOrbiter->GetPosition())));
		m_semiImplicitOrbiter->SetVelocity(2.0f * glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_semiImplicitCentre - m_semiImplicitOrbiter->GetPosition())));
		m_start = true;
	}


	if (m_start) 
	{
		//Counteract gravity
		m_explicitOrbiter->AddForce(glm::vec3(0, m_explicitOrbiter->GetMass() * 9.81, 0));
		m_semiImplicitOrbiter->AddForce(glm::vec3(0, m_semiImplicitOrbiter->GetMass() * 9.81, 0));

		float explicitSpeed = glm::length(m_explicitOrbiter->GetVelocity());
		float semiImplicitSpeed = glm::length(m_semiImplicitOrbiter->GetVelocity());

		//Add cetripetal force (F = mv^2/r)
		m_explicitOrbiter->AddForce(
			(m_explicitOrbiter->GetMass() * explicitSpeed * explicitSpeed) * glm::normalize(m_explicitCentre - m_explicitOrbiter->GetPosition())
			/ m_explicitRadius
		);
		m_semiImplicitOrbiter->AddForce(
			(m_semiImplicitOrbiter->GetMass() * semiImplicitSpeed * semiImplicitSpeed) * glm::normalize(m_semiImplicitCentre - m_semiImplicitOrbiter->GetPosition())
			/ m_semiImplicitRadius
		);

		
		m_explicitDeviations.push_back(glm::length(m_explicitCentre - m_explicitOrbiter->GetPosition()) - 2.0f);
		m_semiImplicitDeviations.push_back(glm::length(m_semiImplicitCentre - m_semiImplicitOrbiter->GetPosition()) - 2.0f);

		if (_input->cmd_x) 
		{
			std::ofstream outputFile("logs/orbitlogs.txt");

			outputFile << "Orbital integration results\nExplicit | Semi-implicit | Verlet\n";

			for (size_t i = 0; i < m_explicitDeviations.size(); i++) 
			{
				outputFile << m_explicitDeviations.at(i) << "		 |		 " << m_semiImplicitDeviations.at(i) << std::endl;
			}

			outputFile.close();
		}
	}

}

void OrbitController::Initialize()
{
	//Explicit object ID = 102
	m_explicitOrbiter = static_pointer_cast<DynamicObject>(m_scene->FindObjectByID(102));
	m_explicitOrbiter->m_integrationMethod = IntegrationMethod::EXPLICITEULER;
	//Euler object ID = 100
	m_semiImplicitOrbiter = static_pointer_cast<DynamicObject>(m_scene->FindObjectByID(100));
	m_semiImplicitOrbiter->m_integrationMethod = IntegrationMethod::IMPLICITEULER;

	//Centre ID = object ID + 1
	m_explicitCentre = m_scene->FindObjectByID(103)->GetPosition();
	m_semiImplicitCentre = m_scene->FindObjectByID(101)->GetPosition();

	//Initialize radii
	m_explicitRadius = glm::distance(m_explicitCentre, m_explicitOrbiter->GetPosition());
	m_semiImplicitRadius = glm::distance(m_semiImplicitCentre, m_semiImplicitOrbiter->GetPosition());

	m_start = false;
	m_fileWrote = false;
}
