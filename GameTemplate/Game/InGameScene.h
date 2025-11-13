#pragma once
#include"IScene.h"
#include "SkyCube.h"
#include"SceneStateContext.h"
#include"Timer.h"



class CoinManager;
class PlacementObject;
class Aircraft;
class ScoreManager;
class Stage;
class InGameStateManager;
class InGameScene : public IScene
{
public:
	InGameScene();
	~InGameScene();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	bool RequestChangeScene(SceneType& type)override;
	void Enter() {};
	void Exit() {};

private:
	void InitInGameContext();
private:
	ModelRender m_bg;				// ”wŒiƒ‚ƒfƒ‹
	Vector3 m_position = Vector3{ 0.0,30.0f,100.0f };				// À•W
	Vector3 m_cameraPosition = Vector3::Zero;		// ƒJƒƒ‰À•W
	Vector3 m_cameraTarget;			// ƒJƒƒ‰’‹“_
	Quaternion m_rotation;			// ‰ñ“]
	Vector3 m_scale = g_vec3One;	// Šg‘å—¦
	Ocean* m_ocean = nullptr;
	SkyCube* m_skyCube = nullptr;
	Aircraft* m_aircraft = nullptr;
	CoinManager* m_coinManager = nullptr;
	ScoreManager* m_scoreManager = nullptr;
	PhysicsStaticObject m_ground;
	Stage* m_stage = nullptr;
	InGameStateManager* m_inGameStateManeger = nullptr;
	InGameContext m_context;
	Timer m_timer;
	SpriteRender test;
};

