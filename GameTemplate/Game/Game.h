#pragma once
#include "SkyCube.h"

class Aircraft;
class Game : public IGameObject
{
public:

	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	/// <summary>
	/// 点を指定した平面に対して鏡映させた位置を返す。
	/// </summary>
	/// <param name="point">反射させたい点</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	inline Vector3 ReflectPointAcrossPlane(const Vector3& point, const Plane& plane) {
		// 平面までの符号付き距離を計算（法線は単位長の前提）
		float dist = plane.SignedDistanceTo(point);

		return point - (plane.GetNormal() * dist * 2.0f);
	}

	/// <summary>
	/// ベクトルを平面に対して反射させる。
	/// </summary>
	/// <param name="v">反射させたい方向ベクトル</param>
	/// <param name="plane">平面（法線とオフセットを持つが、ベクトル反射では法線だけ使う）</param>
	/// <returns>反射後のベクトル</returns>
	inline Vector3 ReflectVectorAcrossPlane(const Vector3& vector, const Plane& plane)
	{
		const Vector3& n = plane.GetNormal();   // 法線ベクトル

		// 射影係数（n方向成分の大きさ / |n|^2）
		const float proj = Dot(n, vector);

		return vector - (n * proj * 2);
	}

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
	Quaternion m_rotation;			// 回転
	Vector3 m_scale = g_vec3One;	// 拡大率
	std::array<AnimationClip, 4> m_animationClipArray;	// アニメーションクリップ配列
	Plane plane = Plane(Vector3{0.0f,1.0f,0.0f}, 0.0f); // 水平面
	Ocean* m_ocean = nullptr;
	bool m_cameraFlag = false;
	float m_timer = 0.0f;
	SkyCube* m_skyCube = nullptr;
	CharacterController m_characterController;
	Vector3 m_moveSpeed = Vector3::Zero;
	float angle = 0.0f;
	Aircraft* m_aircraft = nullptr;
};

