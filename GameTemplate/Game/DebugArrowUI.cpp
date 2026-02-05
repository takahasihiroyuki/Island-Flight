#include "stdafx.h"
#include "DebugArrowUI.h"

namespace
{
	Quaternion FromToRotation(const Vector3& from, const Vector3& to)
	{
		Vector3 f = from;
		Vector3 t = to;
		f.Normalize();
		t.Normalize();

		float cosTheta = Dot(f, t);

		// ほぼ同じ向き
		if (cosTheta > 0.9999f) {
			return Quaternion::Identity;
		}

		// 真逆方向 (180度回転)
		if (cosTheta < -0.9999f) {
			// from に直交する適当な軸を取る
			Vector3 axis = Cross(Vector3(1, 0, 0), f);
			if (axis.LengthSq() < 1e-6f) {
				axis = Cross(Vector3(0, 1, 0), f);
			}
			axis.Normalize();
			Quaternion rot;
			rot.SetRotation(axis, 3.1415f);
			return rot; // 180度
		}

		Vector3 axis = Cross(f, t);
		float s = sqrtf((1.0f + cosTheta) * 2.0f);
		float invS = 1.0f / s;

		Quaternion q;
		q.w = s * 0.5f;
		q.x = axis.x * invS;
		q.y = axis.y * invS;
		q.z = axis.z * invS;
		return q;
	}
}

void DebugArrowUI::OnUpdate()
{
	// ほぼ0なら矢印を出さないなど
	if (m_targetVec.LengthSq() < 1e-6f) {
		// m_momentDebug.SetVisible(false); など
		return;
	}

	//方向を取り出す
	Vector3 dir = m_targetVec;

	float momentLength = m_targetVec.Length();

	dir.Normalize();

	//基準方向（矢印モデルがデフォルトで向いている方向）
	const Vector3 baseDir(1.0f, 0.0f, 0.0f); // モデルが +Z を向いている想定

	//クォータニオンを作成
	Quaternion rot;
	rot = FromToRotation(baseDir, dir);

	// デバッグ用モーメント矢印の更新
	m_arrow.SetRotation(rot);
	m_arrow.SetScale(Vector3(momentLength * m_baseLength * m_baseScale, m_baseScale, m_baseScale));
	m_arrow.SetPosition(m_position);
	m_arrow.Update();
}

void DebugArrowUI::Render(RenderContext& rc)
{
	m_arrow.Draw(rc);
}

void DebugArrowUI::Init(collarType collar, float length, float scale)
{
	switch (collar)
	{
	case enYerrow:
		m_arrow.Init("Assets/UI/arrow/arrowYellow.tkm");
		break;
	case enRed:
		m_arrow.Init("Assets/UI/arrow/arrowRed.tkm");
		break;
	case enBlue:
		m_arrow.Init("Assets/UI/arrow/arrowBlue.tkm");
		break;

	default:
		break;
	}
	m_baseLength = length;
	m_baseScale = scale;

	m_arrow.SetScale(scale * m_baseLength, scale, scale);

}

void DebugArrowUI::OnOpen()
{
}

void DebugArrowUI::OnClose()
{
}
