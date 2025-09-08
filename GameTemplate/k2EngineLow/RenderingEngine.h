#pragma once

#include "Light.h"
#include "PostEffect.h"


namespace nsK2EngineLow {
	class ModelRender;
	class RenderContext;

	class RenderingEngine
	{
	public:
		RenderingEngine();
		~RenderingEngine();
		void Init();

		void InitGBuffer();
		void InitLight();
		void InitMainRenderTarget();
		void InitCopyToframeBufferSprite();
		void InitDefferedLightingSprite();
		/// <summary>
		/// GBuffer�Ƀ����_�����O����B
		/// </summary>
		/// <param name="rc"></param>
		void RenderToGBuffer(RenderContext& rc);

		/// <summary>
		/// �f�B�t�@�[�h�����_�����O���s���B
		/// </summary>
		/// <param name="rc"></param>
		void DeferredLighting(RenderContext& rc);

		/// <summary>
		/// ���C�������_�[�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[����B
		/// </summary>
		/// <param name="rc"></param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

		/// <summary>
		/// �`�悵���I�u�W�F�N�g���N���A����B
		/// </summary>
		void ObjectClear();


		/// <summary>
		/// �`�揈�������s�B
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// �V�[�����C�g�̎擾�B
		/// </summary>
		/// <returns></returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}

		/////////////////////////////////////////////////////
		///�f�B���N�V�������C�g�̊֐�
		/////////////////////////////////////////////////////

		/// <summary>
		/// �f�B���N�V�������C�g�̐ݒ�
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <param name="direction">���C�g�̕���</param>
		/// <param name="color">���C�g�̐F</param>
		void SetDirectionLight(Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight( direction, color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_sceneLight.SetDirLightDirection(direction);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetDirLightColor(Vector3 color)
		{
			m_sceneLight.SetDirLightColor(color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_sceneLight.GetDirLigDirection();
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetDirLigColor() const
		{
			return m_sceneLight.GetDirLigColor();
		}

		void SetAmbientLight(Vector3 ambientColor)
		{
			m_sceneLight.SetAmbientLight(ambientColor);
		}

		/////////////////////////////////////////////////////////////
		//�J�����̊֐�
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// �J�����̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="eyePos">�J�����̍��W</param>
		void SetEyePos(Vector3 eyePos)
		{
			m_sceneLight.SetEyePos(eyePos);
		}

		/// <summary>
		/// ���X�g�Ƀ��f�������_�[��ǉ�����B
		/// </summary>
		/// <param name="modelRender"></param>
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.push_back(modelRender);
		}

	private:
		/// <summary>
		/// �|�X�g�G�t�F�N�g
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void PostEffecting(RenderContext& rc);


	private:
		// GBuffer�ɓ���郌���_�����O�^�[�Q�b�g�̖����B
		enum EnGBuffer
		{
			enGBufferAlbedo,           // �A���x�h
			enGBufferNormal,           // �@��
			enGBufferSpecular,         // �X�y�L����
			enGBufferNum,              // G-Buffer�̐�
		};

		Sprite m_copyToframeBufferSprite;				//���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		Sprite m_diferredLightingSprite;				//�f�B�t�@�[�h���C�e�B���O�p�̃X�v���C�g

		std::array<RenderTarget, enGBufferNum> m_gBuffer;			//GBuffer�p�̃����_�����O�^�[�Q�b�g�B
		RenderTarget m_mainRenderTarget;				//���C�������_�����O�^�[�Q�b�g


		std::vector<ModelRender*>		m_modelList;	//���f�����X�g
		SceneLight						m_sceneLight;	//�V�[�����C�g
		PostEffect						m_postEffect;	//�|�X�g�G�t�F�N�g
	};

}

