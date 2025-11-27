#include "stdafx.h"
#include "ResoltState.h"
#include "ResultUI.h"
#include "UIManager.h"

ResoltState::ResoltState()
{
}

ResoltState::~ResoltState()
{
	DeleteGO(m_resultEnterSE);
	m_resultUI.reset();
}

void ResoltState::OnEnter()
{
	//UI
	m_resultUI = std::make_unique<ResultUI>(m_context->scoreManager);
	m_resultUI->Init();
	m_resultUI->SetDisplayed(true);
	UIManager::GetInstance().RegisterScreen("ResultUI", std::move(m_resultUI));

	//ƒTƒEƒ“ƒh
	m_resultEnterSE = NewGO<SoundSource>(0);
	m_resultEnterSE->Init(static_cast<int>(SoundID::enResultEnterSE));
	m_resultEnterSE->Play(false);
}

void ResoltState::Update()
{
}

void ResoltState::Exit()
{
	UIManager::GetInstance().HideScreen("ResultUI");
}

bool ResoltState::RequestChangeState(InGameStateType& type)
{
	if (g_pad[0]->IsPress(enButtonA))
	{
		type = InGameStateType::enEnd;
		return true;
	}

	return false;
}
