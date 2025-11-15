#include "stdafx.h"
#include "UIManagerObject.h"
#include "UIManager.h"

void UIManagerObject::Render(RenderContext& rc)
{

	UIManager::GetInstance().Render(rc);

}
