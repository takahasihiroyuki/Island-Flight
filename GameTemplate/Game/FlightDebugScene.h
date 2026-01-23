#pragma once
#include"IScene.h"

class DebugArrowUI;
class FlightDebugScene : public IScene
{
public:
	FlightDebugScene() {};
	~FlightDebugScene();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	bool RequestChangeScene(SceneType& type) override;
	void Enter() {};
	void Exit() {};

	/// <summary>
	/// ÉvÉåÉCÉÑÅ[ÇÃì¸óÕ
	/// </summary>
	void PlayerInput();

private:
	Aircraft* m_aircraft = nullptr;
	PhysicsStaticObject m_ground;
	std::vector<DebugArrowUI*> m_debugMomentUI;
	std::vector<DebugArrowUI*> m_debugForceUI;
	std::vector<DebugArrowUI*> m_debugMomentArmUI;
	Ocean* m_ocean = nullptr;
};

