#pragma once
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
	Vector3 m_position;				// 座標
	Quaternion m_rotation;			// 回転
	Vector3 m_scale = g_vec3One;	// 拡大率
	std::array<AnimationClip, 4> m_animationClipArray;	// アニメーションクリップ配列
};

