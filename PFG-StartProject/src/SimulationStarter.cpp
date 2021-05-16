#include "SimulationStarter.h"
#include "Scene.h"

void SimulationStarter::Update(float _deltaTs, Input* _input)
{
	if (_input->cmd_x)
	{
		auto gos = m_scene->GetObjects();
		for (int i = 0; i < gos.size(); i++)
		{
			gos.at(i)->SetSimulated(true);
		}
	}
}

void SimulationStarter::Initialize()
{
}
