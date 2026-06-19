#pragma once
#include"PlacementObject.h"
#include "GameTuningSettings.h"
#include "EffectType.h"

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

	void OnStart() override
	{
		m_addTimeItemSE = NewGO<SoundSource>(0, "AddTimeLimitItemSE");
		m_addTimeItemSE->Init(static_cast<int>(SoundID::enAddTimeLimitItemSE), true);
		m_addTimeItemSE->SetVolume(80);
		m_addTimeItemSE->Play(true);
	}

	void OnUpdate() override
	{
		BonusItemObject::OnUpdate();

		if (m_addTimeItemSE != nullptr)
		{
			m_addTimeItemSE->SetPosition(GetPosition());
		}
	}

	void OnDeactivate() override
	{
		if (m_addTimeItemSE != nullptr)
		{
			m_addTimeItemSE->Stop();
		}
	}

	void ApplyEffect(const BonusItemEffectContext& context) override;

private:
	float m_addSeconds = 0.0f;			//アイテムの効果で加算する秒数。
	SoundSource* m_addTimeItemSE = nullptr;
};

