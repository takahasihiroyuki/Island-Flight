#include "stdafx.h"
#include "SplineMath.h"

Vector3 SplineMath::CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{
	//クランプ
	if (t < 0) t = 0.0;
	if (t > 1) t = 1.0;

	const float t2 = t * t;
	const float t3 = t2 * t;

	// p1を通過するときの進行方向
	const Vector3 startTangent =
		(p2 - p0) * 0.5f;

	// p2を通過するときの進行方向
	const Vector3 endTangent =
		(p3 - p1) * 0.5f;

	// 各位置・接線を、どれくらい使うかを決める係数
	const float startPositionWeight =
		2.0f * t3 - 3.0f * t2 + 1.0f;

	const float startTangentWeight =
		t3 - 2.0f * t2 + t;

	const float endPositionWeight =
		-2.0f * t3 + 3.0f * t2;

	const float endTangentWeight =
		t3 - t2;

	return
		p1 * startPositionWeight +
		startTangent * startTangentWeight +
		p2 * endPositionWeight +
		endTangent * endTangentWeight;
}

Vector3 SplineMath::CatmullRomTangent(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{
	//クランプ
	if (t < 0) t = 0.0;
	if (t > 1) t = 1.0;

	const float t2 = t * t;

	const Vector3 m1 = (p2 - p0) * 0.5f;
	const Vector3 m2 = (p3 - p1) * 0.5f;

	const float h00Derivative =
		6.0f * t2 - 6.0f * t;

	const float h10Derivative =
		3.0f * t2 - 4.0f * t + 1.0f;

	const float h01Derivative =
		-6.0f * t2 + 6.0f * t;

	const float h11Derivative =
		3.0f * t2 - 2.0f * t;

	return
		p1 * h00Derivative +
		m1 * h10Derivative +
		p2 * h01Derivative +
		m2 * h11Derivative;
}

Vector3 SplineMath::CatmullRomDirection(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{
	Vector3 direction = CatmullRomTangent(p0, p1, p2, p3, t);
	direction.Normalize();
	return direction;
}
