#pragma once
#include"IScene.h"

class FlightDebugScene : public IScene
{
	FlightDebugScene();
	~FlightDebugScene();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	Aircraft* m_aircraft = nullptr;
	PhysicsStaticObject m_ground;
};

