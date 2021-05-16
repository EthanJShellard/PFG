#include "PerformanceMonitor.h"
#include "SDL.h"
#include <iostream>
#include <string>

PerformanceMonitor::PerformanceMonitor()
{
	m_totalFrames = 0;
	m_frametimeSum = 0;
	m_minFrametime = 0xFFFFFFFFFFFFFFFF;
	m_maxFrametime = 0;

	m_collisionChecks = 0;
	m_collisionCheckTimeSum = 0;
	m_maxCollisionCheckTime = 0;
	m_minCollisionCheckTime = 0xFFFFFFFFFFFFFFFF;

	m_updates = 0;
	m_updateTimeSum = 0;
	m_maxUpdateTime = 0;
	m_minUpdateTime = 0xFFFFFFFFFFFFFFFF;

	m_frameStartTime = 0;
	m_collisionCheckStartTime = 0;
	m_updateStartTime = 0;
}

void PerformanceMonitor::FrameBegin()
{
	m_frameStartTime = SDL_GetTicks();
}

void PerformanceMonitor::FrameEnd()
{
	m_totalFrames++;

	Uint32 frametime = SDL_GetTicks() - m_frameStartTime;

	if (frametime < m_minFrametime) 
	{
		m_minFrametime = frametime;
	}
	else if (frametime > m_maxFrametime) 
	{
		m_maxFrametime = frametime;
	}

	m_frametimeSum += frametime;
}

void PerformanceMonitor::CollisionBegin()
{
	m_collisionCheckStartTime = SDL_GetTicks();
}

void PerformanceMonitor::CollisionEnd()
{
	m_collisionChecks++;

	Uint32 collisionTime = SDL_GetTicks() - m_collisionCheckStartTime;

	if (collisionTime < m_minCollisionCheckTime) 
	{
		m_minCollisionCheckTime = collisionTime;
	}
	else if (collisionTime > m_maxCollisionCheckTime) 
	{
		m_maxCollisionCheckTime = collisionTime;
	}

	m_collisionCheckTimeSum += collisionTime;
}

void PerformanceMonitor::UpdatesBegin()
{
	m_updateStartTime = SDL_GetTicks();
}

void PerformanceMonitor::UpdatesEnd()
{
	m_updates++;

	Uint32 updateTime = SDL_GetTicks() - m_updateStartTime;

	if (updateTime < m_minUpdateTime)
	{
		m_minUpdateTime = updateTime;
	}
	else if (updateTime > m_maxUpdateTime)
	{
		m_maxUpdateTime = updateTime;
	}

	m_updateTimeSum += updateTime;
}

void PerformanceMonitor::OutputResults()
{
	std::string output;

	double averageFrametime = (double)m_frametimeSum / (m_totalFrames * 1000.0); //convert to seconds and find average
	double averageFramerate = 1 / averageFrametime;

	double averageCollisionCheckTime = (double)m_collisionCheckTimeSum / (m_collisionChecks * 1000.0); //convert to seconds and find average

	double averageObjectUpdateTime = (double)m_updateTimeSum / (m_updates * 1000.0);//convert to seconds and find average

	output.append("__PERFORMANCE METRICS__\n\n");
	//Framerate info
	output.append("FRAMERATE METRICS:\n");
	output.append("Total frames: ");
	output.append(std::to_string(m_totalFrames));
	output.append("\nAverage frametime: ");
	output.append(std::to_string(averageFrametime));
	output.append("\nAverage framerate: ");
	output.append(std::to_string(averageFramerate));
	output.append("\nMinimum frametime: ");
	output.append(std::to_string(m_minFrametime / 1000.0));
	output.append("\nMaximum frametime: ");
	output.append(std::to_string(m_maxFrametime / 1000.0));
	//End of framerate info
	
	//Collision time info
	output.append("\n\nCOLLISION TIME METRICS: (Note: Per object, time until all checks complete)\n");
	output.append("Total collision checks: ");
	output.append(std::to_string(m_collisionChecks));
	output.append("\nAverage check time: ");
	output.append(std::to_string(averageCollisionCheckTime));
	output.append("\nMinimum check time: ");
	output.append(std::to_string(m_minCollisionCheckTime / 1000.0));
	output.append("\nMaximum check time: ");
	output.append(std::to_string(m_maxCollisionCheckTime / 1000.0));
	//End collision time info

	//Object update info
	output.append("\n\nOBJECT UPDATE (Collision Response) TIME METRICS:\n");
	output.append("Total object updates checks: ");
	output.append(std::to_string(m_updates));
	output.append("\nAverage update time: ");
	output.append(std::to_string(averageObjectUpdateTime));
	output.append("\nMinimum update time: ");
	output.append(std::to_string(m_minUpdateTime / 1000.0));
	output.append("\nMaximum update time: ");
	output.append(std::to_string(m_maxUpdateTime / 1000.0));
	//End object update info

	std::cout << output << std::endl;
}
