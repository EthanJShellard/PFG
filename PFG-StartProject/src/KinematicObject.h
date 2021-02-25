#pragma once
#include "GameObject.h"
#include <GLM/gtc/matrix_transform.hpp>
class KinematicObject : public GameObject
{
private:
	bool simulated;


public:

	void UpdateModelMatrix();

	void Update(float deltaTs);

	void SetSimulated(bool sim);

	KinematicObject();
	~KinematicObject();
};

