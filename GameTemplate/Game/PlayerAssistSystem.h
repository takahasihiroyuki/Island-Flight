#pragma once
#include <optional> 
#include "AssistWarpPoint.h"
#include "PlayerAssistUI.h"

class PlayerAssistSystem
{
public:
	PlayerAssistSystem() {};
	~PlayerAssistSystem();
	void Update(Aircraft& aircraft);
	void Init(std::vector<AssistWarpPoint> warpPoint);

private:
	bool ShouldAssist();
	AssistWarpPoint FindBestWarpPoint(Vector3 playerPosition);
private:
	std::vector<AssistWarpPoint> m_warpPoints;
	std::unique_ptr<PlayerAssistUI> m_playerAssistUI;
};

