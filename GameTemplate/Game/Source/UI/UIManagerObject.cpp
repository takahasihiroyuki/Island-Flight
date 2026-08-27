#include "Source/stdafx.h"
#include "Source/UI/UIManagerObject.h"
#include "Source/UI/UIManager.h"

void UIManagerObject::Render(RenderContext& rc)
{

	UIManager::GetInstance().Render(rc);

}
