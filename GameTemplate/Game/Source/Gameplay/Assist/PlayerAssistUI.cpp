#include "Source/stdafx.h"
#include "Source/Gameplay/Assist/PlayerAssistUI.h"
#include "Source/UI/UIManager.h"

PlayerAssistUI::PlayerAssistUI()
{
	m_animDurationOpen = 1;		//UI表示時のアニメーションの時間
	m_animDurationClose = 1;	//UI非表示時のアニメーションの時間
}

void PlayerAssistUI::OnUpdate()
{
	m_ui.SetPosition(m_uiShownPos + m_uiOpenCloseAnimOffset);
	m_ui.Update();
}

void PlayerAssistUI::Render(RenderContext& rc)
{
	m_ui.Draw(rc);
}

void PlayerAssistUI::Init()
{
	m_ui.Init("Assets/UI/PlayerAssistSystem/PlayerAssistSystem.DDS", 294, 182);
	m_ui.SetPosition(m_uiHiddenPos);
	m_ui.Update();
}

void PlayerAssistUI::OnOpen()
{
}

void PlayerAssistUI::OnClose()
{
}

void PlayerAssistUI::OnOpenAnimUpdate(float t)
{
	float positionT = t;

	// 非表示アニメーション
	m_uiOpenCloseAnimOffset.Lerp(positionT, m_uiHiddenPos, m_uiShownPos);
	m_uiOpenCloseAnimOffset -= m_uiShownPos;
}

void PlayerAssistUI::OnCloseAnimUpdate(float t)
{
	float s = 1.70158f;
	float positionT = t * t * ((s + 1.0f) * t - s);

	// 非表示アニメーション
	m_uiOpenCloseAnimOffset.Lerp(positionT, m_uiShownPos, m_uiHiddenPos);
	m_uiOpenCloseAnimOffset -= m_uiShownPos;

}