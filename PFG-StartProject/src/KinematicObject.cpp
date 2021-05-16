#include "KinematicObject.h"

void KinematicObject::UpdateModelMatrix()
{
	m_modelMatrix = glm::translate(glm::mat4(1), m_position);
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
	m_invModelMatrix = glm::inverse(m_modelMatrix);
}

void KinematicObject::Update(float _deltaTs)
{
	if (m_simulated) 
	{
		glm::vec3 g = glm::vec3(0.0, -0.0981, 0.0);

		//v = u + at, simulate gravity
		m_velocity += g * _deltaTs;
		m_position += m_velocity;

		//Fake collision detection
		if (m_position.y <= m_radius)
		{
			m_position.y = m_radius;
			m_velocity = glm::vec3(0.0, 0.0, 0.0);
		}

	}
	UpdateModelMatrix();
}

void KinematicObject::SetRadius(float _newRad)
{
	m_radius = _newRad;
}

void KinematicObject::SetSimulated(bool _sim)
{
	m_simulated = _sim;
}

KinematicObject::KinematicObject()
{
	// Initialise everything here
	m_mesh = NULL;
	m_material = NULL;
	// Set default value
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_simulated = false;
	m_radius = 1.0f;
}

KinematicObject::~KinematicObject()
{
}
