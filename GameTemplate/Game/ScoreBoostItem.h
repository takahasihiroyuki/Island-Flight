#pragma once
#include"PlacementObject.h"
#include "GameTuningSettings.h"

class ScoreBoostItem final : public BonusItemObject
{
public:
	static constexpr const char* GetObjectName()
	{
		return "ScoreBoostItem";
	}

	void InitItem(
		const Vector3& position,
		const GameTuning::ScoreBoostItemConfig& config,
		const BonusItemWaypointSet* waypointSet,
		const Vector3& startPosition,
		float moveDuration)
	{
		m_multiplier = config.multiplier;
		m_duration = config.duration;

		Init(
			"Assets/modelData/Item/scoreBoostItem.tkm",
			position,
			Quaternion::Identity,
			Vector3::One,
			"ScoreBoostItem"
		);

		InitMovement(waypointSet, startPosition, moveDuration);
	}

	const char* GetItemModelName() const override
	{
		return "ScoreBoostItem";
	}

	const char* GetItemModelPath() const override
	{
		return "Assets/modelData/Item/scoreBoostItem.tkm";
	}


protected:
	void ApplyEffect(const BonusItemEffectContext& context) override;

private:

	float m_multiplier = 1.0f;
	float m_duration = 0.0f;
};
