#pragma once
#include"PlacementObject.h"
#include "GameTuningSettings.h"

class Timer;
class AddTimeItem final : public BonusItemObject
{
public:

	static constexpr const char* GetObjectName()
	{
		return "AddTimeItem";
	}

	void InitItem(
		const Vector3& position,
		const GameTuning::AddTimeItemConfig& addTimeConfig,
		const BonusItemWaypointSet* waypointSet,
		const Vector3& startPosition,
		float moveDuration

	)
	{
		m_addSeconds = addTimeConfig.addSeconds;

		Init(
			"Assets/modelData/Item/AddTimeItem.tkm",
			position,
			Quaternion::Identity,
			Vector3::One,
			"AddTimeItem"
		);

		InitMovement(waypointSet, startPosition, moveDuration);
	}

	const char* GetItemModelName() const override
	{
		return "AddTimeItem";
	}

	const char* GetItemModelPath() const override
	{
		return "Assets/modelData/Item/AddTimeItem.tkm";
	}


protected:
	void ApplyEffect(const BonusItemEffectContext& context) override;

private:
	float m_addSeconds = 0.0f;			//アイテムの効果で加算する秒数。
};

