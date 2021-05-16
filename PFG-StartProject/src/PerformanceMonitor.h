#pragma once
#include <cstdint>

class PerformanceMonitor 
{
	//All times in ticks

	uint32_t m_totalFrames;
	uint32_t m_frametimeSum;
	uint32_t m_minFrametime;
	uint32_t m_maxFrametime;
	
	//Per colliding object
	uint32_t m_collisionChecks;
	uint32_t m_collisionCheckTimeSum;
	uint32_t m_maxCollisionCheckTime;
	uint32_t m_minCollisionCheckTime;
	//Per update
	uint32_t m_updates;
	uint32_t m_updateTimeSum;
	uint32_t m_maxUpdateTime;
	uint32_t m_minUpdateTime;

	uint32_t m_frameStartTime;
	uint32_t m_collisionCheckStartTime;
	uint32_t m_updateStartTime;

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