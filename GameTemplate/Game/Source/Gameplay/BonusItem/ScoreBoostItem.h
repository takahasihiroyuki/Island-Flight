#pragma once
#include"Source/Gameplay/Stage/PlacementObject.h"
#include "Source/Core/GameTuningSettings.h"
#include"Source/Effect/EffectType.h"

namespace {
	Vector3 AURAEFFECT_SCALE = Vector3{ 30,30,120 };
}

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

		StartAuraEffect();
	}

	void Activate() override
	{
		BonusItemObject::Activate();
		StartAuraEffect();
	}

	const char* GetItemModelName() const override
	{
		return "ScoreBoostItem";
	}

	const char* GetItemModelPath() const override
	{
		return "Assets/modelData/Item/scoreBoostItem.tkm";
	}


private:
	void OnUpdate() override
	{
		BonusItemObject::OnUpdate();

		if (m_auraEffect)
		{
			//アイテムの向きと逆向きのクォータニオンを作る
			Quaternion reverseRot;
			reverseRot.SetRotationY(Math::PI);
			Quaternion auraRot;
			auraRot.Multiply(reverseRot, GetRotation());

			m_auraEffect->SetPosition(GetPosition());
			m_auraEffect->SetRotation(auraRot);
		}
	}

	void OnDeactivate() override
	{
		if (m_auraEffect)
		{
			m_auraEffect->Stop();
			DeleteGO(m_auraEffect);
			m_auraEffect = nullptr;
		}
	}

	void ApplyEffect(const BonusItemEffectContext& context) override;

	void StartAuraEffect()
	{
		//エフェクトを登録
		EffectEngine::GetInstance()->RegisterEffect(
			enScoreBoostAuraEffect,
			effectPath[enScoreBoostAuraEffect]
		);

		if (m_auraEffect == nullptr)
		{
			m_auraEffect = NewGO<EffectEmitter>(0);
			m_auraEffect->Init(enScoreBoostAuraEffect);
			m_auraEffect->SetScale(AURAEFFECT_SCALE);
			m_auraEffect->SetRotation(Quaternion::Identity);
			m_auraEffect->Play();
		}

		m_auraEffect->SetPosition(GetPosition());
	}

private:

	float m_multiplier = 1.0f;
	float m_duration = 0.0f;
	EffectEmitter* m_auraEffect = nullptr;
};
