#include "Source/stdafx.h"
#include "Source/Scene/Title/TitleUI.h"
#include"Source/UI/UIManager.h"
#include"Source/Gameplay/Time/Timer.h"

namespace {
	const Vector3 ScaleOffsetLogo = Vector3(10.0f, 5.0f, 0.0f);
	const Vector3 BaseScaleStartUI = Vector3(4.0f, 2.0f, 0.0f);
	const Vector3 ScaleOffsetScreen = Vector3(20.0f, 15.0f, 0.0f);

}


TitleUI::TitleUI()
{
	m_animDurationOpen = 5;	//UI表示時のアニメーションの時間
	m_animDurationClose = 1;	//UI非表示時のアニメーションの時間
}

TitleUI::~TitleUI()
{
	DeleteGO(m_timer);
}

void TitleUI::OnUpdate()
{
	float t = m_timer->GetElapsedTime();

	m_startUIScale = BaseScaleStartUI;
	m_startUIScale += Vector3::One * 0.5 * abs(std::sin(t * 4));


	m_titleLogo.SetPosition(m_logoShownPos + m_logoOpenCloseAnimOffset);
	m_titleLogo.SetScale(ScaleOffsetLogo);
	m_titleLogo.Update();

	m_titleStart.SetPosition(m_startUIPosBase + m_StartUIOpenCloseAnimOffset);
	if (m_state != UIState::enClosing) {
		m_titleStart.SetScale(m_startUIScale);
	}
	else {
		m_titleStart.SetScale(m_StartUICloseAnimScaleOffset);
	}
	m_titleStart.Update();


	m_titleScreen.SetScale(ScaleOffsetScreen);
	m_titleScreen.Update();
}

void TitleUI::Render(RenderContext& rc)
{
	//m_titleScreen.Draw(rc);
	m_titleLogo.Draw(rc);
	m_titleStart.Draw(rc);

}

void TitleUI::Init()
{
	m_titleLogo.Init("Assets/UI/Title/TitleRogo.DDS", 100.0f, 100.0f);
	m_titleLogo.Update();
	m_titleScreen.Init("Assets/UI/Title/TitleScren.DDS", 100.0f, 100.0f);
	m_titleScreen.Update();

	m_titleStart.Init("Assets/UI/Title/TitleStart.DDS", 100.0f, 100.0f);
	m_titleStart.SetPivot(Vector2(0.5f, 0.5f));
	m_titleStart.Update();

	m_timer = NewGO<Timer>(0);
}

void TitleUI::OnOpen()
{
	//タイマーを動かす
	m_timer->SetRunning(true);
}

void TitleUI::OnClose()
{
}

void TitleUI::OnOpenAnimUpdate(float t)
{

	m_logoOpenCloseAnimOffset = m_logoShownPos;
	m_logoOpenCloseAnimOffset.y = m_logoShownPos.y + 150 * abs(cos(t * 30)) * std::exp(-t * 5);
	m_logoOpenCloseAnimOffset.y -= m_logoShownPos.y;
}

void TitleUI::OnCloseAnimUpdate(float t)
{
	float s = 1.70158f;
	float logoT = t * t * ((s + 1.0f) * t - s);
	float startT = t * t * t * ((s + 1.0f) * t - s);

	// ロゴの非表示アニメーション
	m_logoOpenCloseAnimOffset.Lerp(logoT, m_logoShownPos, m_logoHiddenPos);
	m_logoOpenCloseAnimOffset.y -= m_logoShownPos.y;

	// スタートUIの非表示アニメーション
	m_StartUICloseAnimScaleOffset.Lerp(startT, BaseScaleStartUI, m_startUIHiddenScale);
}