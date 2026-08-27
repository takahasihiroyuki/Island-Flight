#include "Source/stdafx.h"
#include "Source/Gameplay/BonusItem/BonusItemMover.h"
#include "Source/Gameplay/BonusItem/BonusItemWaypointSet.h"
#include "Source/Math/SplineMath.h"
#include <random>


bool BonusItemMover::Init(
	const BonusItemWaypointSet* waypointSet,
	const Vector3& startPosition,
	float moveDuration)
{
	if (waypointSet == nullptr)return false;

	// ポイントが2つ以上あることを確認する。
	if (waypointSet->GetPointCount() < 2)return false;

	// 移動にかかる時間が正の値であることを確認する。
	if (moveDuration <= 0.0f)return false;

	m_waypointSet = waypointSet;
	m_moveDuration = moveDuration;

	//最初は前の位置がないので、現在の位置と同じにしておく。
	m_previousPosition = startPosition;
	m_currentPosition = startPosition;

	m_previousPointIndex = -1;
	m_currentPointIndex = -1;

	// 最初の移動先を選ぶ。
	m_nextPointIndex = SelectNextPointIndex(-1, -1);

	//次のポイントのインデックスがちゃんとした値か
	if (m_nextPointIndex < 0)
	{
		m_waypointSet = nullptr;
		return false;
	}

	// その次の移動先を選ぶ。
	// 最初に選んだ点と同じ点は除外する。
	m_afterNextPointIndex = SelectNextPointIndex(
		m_nextPointIndex,
		-1
	);

	if (m_afterNextPointIndex < 0)
	{
		m_waypointSet = nullptr;
		return false;
	}

	m_progress = 0.0f;
	m_position = startPosition;

	return true;
}

void BonusItemMover::Update(float deltaTime)
{
	if (m_waypointSet == nullptr)
	{
		return;
	}

	//すでに現在区間の移動を終えているなら、次の区間に切り替える
	//念のため進行度が2以上になったりする場合に備えてifじゃなくてwhileを使っている
	while (m_progress >= 1.0f)
	{
		m_progress -= 1.0f;

		//次の区間に切り替える
		if (!AdvanceToNextSegment())
		{
			//次の区間がないならその区間の最後の点で止める
			m_progress = 1.0f;

			m_position =
				m_waypointSet->GetPoint(
					m_nextPointIndex
				);

			return;
		}
	}

	// 進行度を更新する。
	m_progress += deltaTime / m_moveDuration;

	//それぞれのポイントの位置を取得する。
	const Vector3& p0 = m_previousPosition;
	const Vector3& p1 = m_currentPosition;
	const Vector3& p2 = m_waypointSet->GetPoint(m_nextPointIndex);
	const Vector3& p3 = m_waypointSet->GetPoint(m_afterNextPointIndex);

	// キャットムルロム曲線に沿って位置を更新する。
	m_position = SplineMath::CatmullRom(
		p0,
		p1,
		p2,
		p3,
		m_progress
	);

	m_direction = SplineMath::CatmullRomDirection(
		p0,
		p1,
		p2,
		p3,
		m_progress
	);

	//ローテーションを更新
	Quaternion targetRotation;
	targetRotation.SetRotation(
		Vector3::Front,
		m_direction
	);

	m_rotation = targetRotation;
}

std::vector<int> BonusItemMover::CreateSelectablePointIndices(int currentPointIndex, int previousPointIndex) const
{
	//次のインデックスの候補
	std::vector<int> candidates;

	if (m_waypointSet == nullptr)
	{
		return candidates;
	}

	//ポイントの個数を取得
	const int pointCount = m_waypointSet->GetPointCount();

	for (int index = 0; index < pointCount; ++index)
	{
		//今のポイントもしくは前のポイントなら除外する
		if (index == currentPointIndex
			|| index == previousPointIndex)continue;


		candidates.push_back(index);
	}

	return candidates;
}

int BonusItemMover::SelectRandomPointIndex(
	const std::vector<int>& candidates
) const
{

	if (candidates.empty())
	{
		return -1;
	}


	//候補の中からランダムに選ぶ
	static std::random_device randomDevice;
	static std::mt19937 randomEngine(
		randomDevice()
	);

	std::uniform_int_distribution<int> distribution(
		0,
		static_cast<int>(candidates.size()) - 1
	);

	const int selectedCandidateIndex =
		distribution(randomEngine);

	return candidates[selectedCandidateIndex];
}

bool BonusItemMover::AdvanceToNextSegment()
{
	//今向かっている点
	const int reachedPointIndex = m_nextPointIndex;

	//その次に向かうことが決まっている点
	const int followingPointIndex = m_afterNextPointIndex;

	//さらにその先の点を選ぶ
	//followingPointIndex自身と、直前のreachedPointIndexは除外する。
	int newAfterNextPointIndex =
		SelectNextPointIndex(
			followingPointIndex,
			reachedPointIndex
		);

	// 候補がなければ、直前の点へ戻ることを許可する。
	if (newAfterNextPointIndex < 0)
	{
		newAfterNextPointIndex =
			SelectNextPointIndex(
				followingPointIndex,
				-1
			);
	}

	if (newAfterNextPointIndex < 0)
	{
		return false;
	}

	//新しく選んだポイントを次の次のポイントにしてそれぞれのポイントをずらす
	{
		m_previousPosition =
			m_currentPosition;

		m_currentPosition =
			m_waypointSet->GetPoint(
				reachedPointIndex
			);

		m_previousPointIndex =
			m_currentPointIndex;

		m_currentPointIndex =
			reachedPointIndex;

		m_nextPointIndex =
			followingPointIndex;

		m_afterNextPointIndex =
			newAfterNextPointIndex;
	}

	return true;
}

