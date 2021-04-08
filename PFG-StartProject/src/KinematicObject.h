#pragma once
#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
class KinematicObject : public GameObject
{
private:
	bool simulated;
	float radius;

public:

	void UpdateModelMatrix();

	void Update(float deltaTs);

	void SetRadius(float newRad);

	void SetSimulated(bool sim);

	KinematicObject();
	~KinematicObject();
};

