#pragma once
#include <optional> 
#include "AssistWarpPoint.h"
class PlayerAssistSystem
{
public:
	PlayerAssistSystem(std::vector<AssistWarpPoint> warpPoint) :m_warpPoints(warpPoint) {};
	PlayerAssistSystem() {};
	~PlayerAssistSystem() {};
	void Update(Aircraft& aircraft);

private:
	bool ShouldAssist();
	AssistWarpPoint FindBestWarpPoint(Vector3 playerPosition);
private:
	std::vector<AssistWarpPoint> m_warpPoints;
};

