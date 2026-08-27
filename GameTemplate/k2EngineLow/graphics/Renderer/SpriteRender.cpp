#include "k2EngineLowPreCompile.h"
#include "graphics/Renderer/SpriteRender.h"

namespace nsK2EngineLow {

	void SpriteRender::Init(const char* filePath, const float width, const float height, AlphaBlendMode alphaBlendMode)
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_ddsFilePath[0] = filePath;
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_width = static_cast<UINT>(width);
		spriteInitData.m_height = static_cast<UINT>(height);
		spriteInitData.m_alphaBlendMode = alphaBlendMode;
		//spriteInitData.m_expandConstantBuffer = &m_spriteCB;
		//spriteInitData.m_expandConstantBufferSize = sizeof(m_spriteCB);


		//sprite初期化オブジェクトを使って、spriteを初期化する
		m_sprite.Init(spriteInitData);

		m_isInit = true;

	}

	void SpriteRender::Draw(RenderContext& rc)
	{
		if (!m_isInit)return;
		g_renderingEngine->AddSpriteRenderList(this);

	}

}