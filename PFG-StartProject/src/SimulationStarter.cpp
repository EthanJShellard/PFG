#include "SimulationStarter.h"
#include "Scene.h"

void SimulationStarter::Update(float deltaTs, Input* input)
{
	if (input->cmd_x)
	{
		auto gos = scene->GetObjects();
		for (int i = 0; i < gos.size(); i++)
		{
			gos.at(i)->SetSimulated(true);
		}
	}
}

void SimulationStarter::Initialize()
{
}
