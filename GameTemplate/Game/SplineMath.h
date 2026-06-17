#pragma once
/// <summary>
/// キャットムルロム曲線に関する数学関数を提供するクラス。
/// </summary>

namespace SplineMath
{
	/// <summary>
	/// キャットムルロム曲線状の位置を計算する
	/// p1とp2の間を補完する。
	/// </summary>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatmullRom(
		const Vector3& p0,
		const Vector3& p1,
		const Vector3& p2,
		const Vector3& p3,
		float t
	);

	/// <summary>
	/// キャットムルロム曲線状の接線を計算する
	/// </summary>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatmullRomTangent(
		const Vector3& p0,
		const Vector3& p1,
		const Vector3& p2,
		const Vector3& p3,
		float t
	);

	/// <summary>
	/// キャットムルロム曲線状の進行方向を計算する
	/// </summary>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatmullRomDirection(
		const Vector3& p0,
		const Vector3& p1,
		const Vector3& p2,
		const Vector3& p3,
		float t
	);
}
