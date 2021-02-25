#include "KinematicObject.h"

void KinematicObject::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(glm::mat4(1), _position);
	_modelMatrix = glm::scale(_modelMatrix, _scale);
	_invModelMatrix = glm::inverse(_modelMatrix);
}

void KinematicObject::Update(float deltaTs)
{
	if (simulated) 
	{
		glm::vec3 g = glm::vec3(0.0, -0.0981, 0.0);

		//v = u + at, simulate gravity
		_velocity += g * deltaTs;
		_position += _velocity;

		//Fake collision detection
		if (_position.y <= radius)
		{
			_position.y = radius;
			_velocity = glm::vec3(0.0, 0.0, 0.0);
		}

	}
	UpdateModelMatrix();
}

void KinematicObject::SetSimulated(bool sim)
{
	simulated = sim;
}

void KinematicObject::SetRadius(float newRad)
{
	radius = newRad;
}

KinematicObject::KinematicObject()
{
	// Initialise everything here
	_mesh = NULL;
	_material = NULL;
	// Set default value
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	simulated = false;
	radius = 1.0f;
}

KinematicObject::~KinematicObject()
{
}
