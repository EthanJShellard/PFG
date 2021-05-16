#pragma once
#include "Script.h"
#include "Input.h"


class SimulationStarter : public Script
{
	void Update(float _deltaTs, Input* _input) override;
	void Initialize() override;
};