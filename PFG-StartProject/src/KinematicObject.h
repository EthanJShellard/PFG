#pragma once
#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
class KinematicObject : public GameObject
{
private:
	bool m_simulated;
	float m_radius;

public:

	void UpdateModelMatrix();

	void Update(float m_deltaTs);

	void SetRadius(float m_newRad);

	void SetSimulated(bool m_sim);

	KinematicObject();
	~KinematicObject();
};

