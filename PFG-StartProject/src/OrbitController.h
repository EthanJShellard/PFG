#pragma once
#include "Script.h"
#include <memory>
#include "glm/glm.hpp"

class DynamicObject;
class Input;

class OrbitController : public Script
{
	bool start;

	std::shared_ptr<DynamicObject> rk4Orbiter;
	std::shared_ptr<DynamicObject> rk2Orbiter;
	std::shared_ptr<DynamicObject> eulerOrbiter;

	glm::vec3 rk4Centre;
	glm::vec3 rk2Centre;
	glm::vec3 eulerCentre;

	float rk4Radius;
	float rk2Radius;
	float eulerRadius;

	void Update(float deltaTs, Input* input) override;
	void Initialize() override;
};