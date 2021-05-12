#pragma once
#include <cstdint>

class PerformanceMonitor 
{
	//All times in ticks

	uint32_t totalFrames;
	uint32_t frametimeSum;
	uint32_t minFrametime;
	uint32_t maxFrametime;
	
	//Per colliding object
	uint32_t collisionChecks;
	uint32_t collisionCheckTimeSum;
	uint32_t maxCollisionCheckTime;
	uint32_t minCollisionCheckTime;

	uint32_t updates;
	uint32_t updateTimeSum;
	uint32_t maxUpdateTime;
	uint32_t minUpdateTime;

	uint32_t frameStartTime;
	uint32_t collisionCheckStartTime;
	uint32_t updateStartTime;

public:

	PerformanceMonitor();

	void FrameBegin();
	void FrameEnd();
	void CollisionBegin();
	void CollisionEnd();
	void UpdatesBegin();
	void UpdatesEnd();

	void OutputResults();
};