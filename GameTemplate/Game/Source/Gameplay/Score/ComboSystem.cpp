#include "Source/stdafx.h"
#include "Source/Gameplay/Score/ComboSystem.h"

void ComboSystem::Update()
{
	m_comboTimer.Update();
	// 時間切れかどうかをチェック
	if (m_comboTimer.IsTimeUp())
	{
		ResetCombo();
	}
}

void ComboSystem::AddCombo()
{

	if (m_comboTimer.IsTimeUp())
	{
		m_comboCount = 0;
	}

	m_comboCount++;
	m_comboTimer.Reset();
	m_comboTimer.SetRunning(true);
}

void ComboSystem::ResetCombo()
{
	m_comboCount = 0;
	m_comboTimer.SetRunning(false);
	m_comboTimer.Reset();
}
