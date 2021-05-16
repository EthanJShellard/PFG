#pragma once
#include "Script.h"
#include <memory>
#include <vector>
#include "glm/glm.hpp"

class DynamicObject;
class Input;

class OrbitController : public Script
{
	//Has the simulation been started by the user
	bool m_start;
	//Have we written to the file yet
	bool m_fileWrote;

	//Game objects which will orbit a centre point in a circular motion
	std::shared_ptr<DynamicObject> m_explicitOrbiter;
	std::shared_ptr<DynamicObject> m_semiImplicitOrbiter;

	//Centres around which orbiters will orbit
	glm::vec3 m_explicitCentre;
	glm::vec3 m_semiImplicitCentre;

	//Desired radii of orbits
	float m_explicitRadius;
	float m_semiImplicitRadius;

	//Record of deviations from desired orbit
	std::vector<float> m_explicitDeviations;
	std::vector<float> m_semiImplicitDeviations;

	void Update(float _deltaTs, Input* _input) override;
	void Initialize() override;
};