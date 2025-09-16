#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace nsK2EngineLow
{
	void Shadow::Init()
	{
		// �V���h�E�}�b�v�p�����_�����O�^�[�Q�b�g�̍쐬�B
		shadowMapTarget.Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

	}

	void Shadow::Execute(RenderContext& rc, std::vector<ModelRender*>& obj)
	{
		//�^�[�Q�b�g���V���h�E�}�b�v�ɕύX
		rc.WaitUntilToPossibleSetRenderTarget(shadowMapTarget);
		rc.SetRenderTargetAndViewport(shadowMapTarget);
		rc.ClearRenderTargetView(shadowMapTarget);

		// �܂Ƃ߂ĉe���f�������_�[��`��
		for (auto MobjData : obj)
		{
			//�e���f���̕`��
			MobjData->OnRenderShadowMap(rc, g_renderingEngine->GetLightCamera());
		}

		rc.WaitUntilFinishDrawingToRenderTarget(shadowMapTarget);
	}
}

