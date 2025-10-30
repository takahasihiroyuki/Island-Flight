#pragma once
#include"IScene.h"
#include "SkyCube.h"

class CoinManager;
class PlacementObject;
class Aircraft;
class ScoreManager;
class Stage;
class InGameScene : public IScene
{
public:
	InGameScene();
	~InGameScene();
	virtual bool Start() override;
	virtual void OnUpdate() override;
	virtual void Render(RenderContext& rc) override;
	virtual void CheckChangeScene() override;

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

};

