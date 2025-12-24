#pragma once
#include"IScene.h"

class FlightDebugScene : public IScene
{


	Aircraft* m_aircraft = nullptr;
	PhysicsStaticObject m_ground;
};

