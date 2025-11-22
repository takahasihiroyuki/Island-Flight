#include "stdafx.h"
#include "ResoltState.h"
#include "ResultUI.h"
#include "UIManager.h"

ResoltState::ResoltState()
{
}

ResoltState::~ResoltState()
{
	m_resultUI.reset();
}

void ResoltState::OnEnter()
{
	m_resultUI = std::make_unique<ResultUI>(m_context->scoreManager);
	m_resultUI->Init();
	m_resultUI->SetDisplayed(true);
	UIManager::GetInstance().RegisterScreen("ResultUI", std::move(m_resultUI));

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
