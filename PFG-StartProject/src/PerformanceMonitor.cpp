#include "PerformanceMonitor.h"
#include "SDL.h"
#include <iostream>
#include <string>

PerformanceMonitor::PerformanceMonitor()
{
	totalFrames = 0;
	frametimeSum = 0;
	minFrametime = 0xFFFFFFFFFFFFFFFF;
	maxFrametime = 0;

	collisionChecks = 0;
	collisionCheckTimeSum = 0;
	maxCollisionCheckTime = 0;
	minCollisionCheckTime = 0xFFFFFFFFFFFFFFFF;

	updates = 0;
	updateTimeSum = 0;
	maxUpdateTime = 0;
	minUpdateTime = 0xFFFFFFFFFFFFFFFF;

	frameStartTime = 0;
	collisionCheckStartTime = 0;
	updateStartTime = 0;
}

void PerformanceMonitor::FrameBegin()
{
	frameStartTime = SDL_GetTicks();
}

void PerformanceMonitor::FrameEnd()
{
	totalFrames++;

	Uint32 frametime = SDL_GetTicks() - frameStartTime;

	if (frametime < minFrametime) 
	{
		minFrametime = frametime;
	}
	else if (frametime > maxFrametime) 
	{
		maxFrametime = frametime;
	}

	frametimeSum += frametime;
}

void PerformanceMonitor::CollisionBegin()
{
	collisionCheckStartTime = SDL_GetTicks();
}

void PerformanceMonitor::CollisionEnd()
{
	collisionChecks++;

	Uint32 collisionTime = SDL_GetTicks() - collisionCheckStartTime;

	if (collisionTime < minCollisionCheckTime) 
	{
		minCollisionCheckTime = collisionTime;
	}
	else if (collisionTime > maxCollisionCheckTime) 
	{
		maxCollisionCheckTime = collisionTime;
	}

	collisionCheckTimeSum += collisionTime;
}

void PerformanceMonitor::UpdatesBegin()
{
	updateStartTime = SDL_GetTicks();
}

void PerformanceMonitor::UpdatesEnd()
{
	updates++;

	Uint32 updateTime = SDL_GetTicks() - updateStartTime;

	if (updateTime < minUpdateTime)
	{
		minUpdateTime = updateTime;
	}
	else if (updateTime > maxUpdateTime)
	{
		maxUpdateTime = updateTime;
	}

	updateTimeSum += updateTime;
}

void PerformanceMonitor::OutputResults()
{
	std::string output;

	double averageFrametime = (double)frametimeSum / (totalFrames * 1000.0); //convert to seconds and find average
	double averageFramerate = 1 / averageFrametime;

	double averageCollisionCheckTime = (double)collisionCheckTimeSum / (collisionChecks * 1000.0); //convert to seconds and find average

	double averageObjectUpdateTime = (double)updateTimeSum / (updates * 1000.0);//convert to seconds and find average

	output.append("__PERFORMANCE METRICS__\n\n");
	//Framerate info
	output.append("FRAMERATE METRICS:\n");
	output.append("Total frames: ");
	output.append(std::to_string(totalFrames));
	output.append("\nAverage frametime: ");
	output.append(std::to_string(averageFrametime));
	output.append("\nAverage framerate: ");
	output.append(std::to_string(averageFramerate));
	output.append("\nMinimum frametime: ");
	output.append(std::to_string(minFrametime / 1000.0));
	output.append("\nMaximum frametime: ");
	output.append(std::to_string(maxFrametime / 1000.0));
	//End of framerate info
	
	//Collision time info
	output.append("\n\nCOLLISION TIME METRICS: (Note: Per object, time until all checks complete)\n");
	output.append("Total collision checks: ");
	output.append(std::to_string(collisionChecks));
	output.append("\nAverage check time: ");
	output.append(std::to_string(averageCollisionCheckTime));
	output.append("\nMinimum check time: ");
	output.append(std::to_string(minCollisionCheckTime / 1000.0));
	output.append("\nMaximum check time: ");
	output.append(std::to_string(maxCollisionCheckTime / 1000.0));
	//End collision time info

	//Object update info
	output.append("\n\nOBJECT UPDATE (Collision Response) TIME METRICS:\n");
	output.append("Total object updates checks: ");
	output.append(std::to_string(updates));
	output.append("\nAverage update time: ");
	output.append(std::to_string(averageObjectUpdateTime));
	output.append("\nMinimum update time: ");
	output.append(std::to_string(minUpdateTime / 1000.0));
	output.append("\nMaximum update time: ");
	output.append(std::to_string(maxUpdateTime / 1000.0));
	//End object update info

	std::cout << output << std::endl;
}
