#pragma once
#include "SkyCube.h"
class CoinManager;
class PlacementObject;
class Aircraft;
class ScoreManager;
class Stage;
class Game : public IGameObject
{
public:

	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

private:
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : 待機アニメーション、
		enAnimClip_Run,		// 1 : 走りアニメーション。
		enAnimClip_Num,		// 2 :アニメーションクリップの数。
	};

	ModelRender m_model;					// モデル
	ModelRender m_bg;				// 背景モデル
	ModelRender m_island;			// 島モデル
	Vector3 m_position = Vector3{0.0,30.0f,100.0f};				// 座標
	Vector3 m_cameraPosition=Vector3::Zero;		// カメラ座標
	Vector3 m_cameraTarget;			// カメラ注視点
	Quaternion /*m_rotation*/;			// 回転
	Vector3 m_scale = g_vec3One;	// 拡大率
	std::array<AnimationClip, 4> m_animationClipArray;	// アニメーションクリップ配列
	Ocean* m_ocean = nullptr;
	bool m_cameraFlag = false;
	float m_timer = 0.0f;
	SkyCube* m_skyCube = nullptr;
	CharacterController m_characterController;
	Vector3 m_moveSpeed = Vector3::Zero;
	float angle = 0.0f;
	Aircraft* m_aircraft = nullptr;
	std::vector<PlacementObject*> m_placementObject;

	CoinManager* m_coinManager=nullptr;
	ScoreManager* m_scoreManager = nullptr;
	PhysicsStaticObject m_ground;
	Stage* m_stage = nullptr;
};

