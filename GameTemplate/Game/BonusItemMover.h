#pragma once

class BonusItemWaypointSet;

class BonusItemMover
{
public:
	/// <summary>
	/// ボーナスアイテムの移動を初期化する。
	/// </summary>
	/// <param name="waypointSet"></param>
	/// <param name="previousPointIndex">ひとつ前のポイント</param>
	/// <param name="currentPointIndex">今のポイント</param>
	/// <param name="nextPointIndex">次のポイント</param>
	/// <param name="afterNextPointIndex">次の次のポイント</param>
	/// <param name="moveDuration">移動にかかる時間</param>
	/// <returns></returns>
	bool Init(
		const BonusItemWaypointSet* waypointSet,
		const Vector3& startPosition,
		float moveDuration
	);

	void Update(float deltaTime);

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	const Vector3& GetDirection() const
	{
		return m_direction;
	}

	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

private:
	/// <summary>
/// 次に行く可能性のあるポイントの一覧を作る
/// </summary>
/// <param name="excludedPointIndex1"></param>
/// <param name="excludedPointIndex2"></param>
/// <returns></returns>
	std::vector<int> CreateSelectablePointIndices(
		int currentPointIndex,
		int previousPointIndex
	) const;

	/// <summary>
	/// 次のポイントをランダムに選びインデックスを返す
	/// 候補一覧から次のポイントをランダムに選ぶ
	/// </summary>
	/// <param name="currentPointIndex"></param>
	/// <param name="previousPointIndex"></param>
	/// <returns></returns>
	int SelectRandomPointIndex(
		const std::vector<int>& candidates
	) const;

	/// <summary>
	/// 次のポイントのインデックスを返す
	/// 今のポイント、前のポイントがない時は-1などを入れる
	/// </summary>
	/// <param name="currentPointIndex"></param>
	/// <param name="previousPointIndex"></param>
	/// <returns></returns>
	int SelectNextPointIndex(
		int currentPointIndex,
		int previousPointIndex
	) const
	{
		const std::vector<int> candidates =
			CreateSelectablePointIndices(
				currentPointIndex,
				previousPointIndex
			);

		return SelectRandomPointIndex(candidates);
	}

	/// <summary>
	/// 今の区間を終了して、次の区間に切り替える
	/// 今の区間が進行し終わったら（m_progressが１になったら）呼びます
	/// </summary>
	/// <returns></returns>
	bool AdvanceToNextSegment();

private:
	const BonusItemWaypointSet* m_waypointSet = nullptr;

	Vector3 m_previousPosition = Vector3::Zero;
	Vector3 m_currentPosition = Vector3::Zero;

	int m_previousPointIndex = -1;		//ひとつ前の場所のインデックス。
	int m_currentPointIndex = -1;		//現在の場所のインデックス。
	int m_nextPointIndex = -1;			//次の場所のインデックス。
	int m_afterNextPointIndex = -1;		//次の次の場所のインデックス。

	float m_progress = 0.0f;			// 0.0f～1.0fの範囲で、現在のポイントから次のポイントへの移動の進行度を表す値。
	float m_moveDuration = 1.0f;		// 移動にかかる時間。

	Vector3 m_position = Vector3::Zero;
	Vector3 m_direction = Vector3::Front;
	Quaternion m_rotation;
};