#pragma once
#include "Script.h"
#include <memory>
#include <vector>
#include "glm/glm.hpp"

class DynamicObject;
class Input;

class OrbitController : public Script
{
	bool m_start;
	bool m_fileWrote;

	std::shared_ptr<DynamicObject> m_explicitOrbiter;
	std::shared_ptr<DynamicObject> m_semiImplicitOrbiter;

	glm::vec3 m_explicitCentre;
	glm::vec3 m_semiImplicitCentre;

	float m_explicitRadius;
	float m_semiImplicitRadius;

	std::vector<float> m_explicitDeviations;
	std::vector<float> m_semiImplicitDeviations;

	void Update(float _deltaTs, Input* _input) override;
	void Initialize() override;
};