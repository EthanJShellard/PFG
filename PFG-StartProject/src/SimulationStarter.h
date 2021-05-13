#pragma once
#include "Script.h"
#include "Input.h"


class SimulationStarter : public Script
{
	void Update(float deltaTs, Input* input) override;
	void Initialize() override;
};