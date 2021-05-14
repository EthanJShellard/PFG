#pragma once
#include "Script.h"
#include <memory>
#include <vector>
#include "glm/glm.hpp"

class DynamicObject;
class Input;

class OrbitController : public Script
{
	bool start;
	bool fileWrote;

	std::shared_ptr<DynamicObject> explicitOrbiter;
	std::shared_ptr<DynamicObject> semiImplicitOrbiter;

	glm::vec3 explicitCentre;
	glm::vec3 semiImplicitCentre;

	float explicitRadius;
	float semiImplicitRadius;

	std::vector<float> explicitSpeeds;
	std::vector<float> semiImplicitSpeeds;

	void Update(float deltaTs, Input* input) override;
	void Initialize() override;
};