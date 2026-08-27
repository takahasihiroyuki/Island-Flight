#pragma once

/// <summary>
/// ボーナスアイテムが移動先として使用するウェイポイントの集合
/// </summary>
class BonusItemWaypointSet
{
public:
	void AddPoint(const Vector3& point)
	{
		m_points.push_back(point);
	}

	int GetPointCount() const
	{
		return static_cast<int>(m_points.size());
	}

	/// <summary>
	/// 指定されたインデックスが有効かどうかを判定する。
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	bool IsValidIndex(int index) const
	{
		// インデックスが0以上で、ポイント数未満であれば有効とみなす。
		return index >= 0 &&
			index < GetPointCount();
	}

	const Vector3& GetPoint(int index) const
	{
		return m_points[index];
	}

private:
	std::vector<Vector3> m_points;
};