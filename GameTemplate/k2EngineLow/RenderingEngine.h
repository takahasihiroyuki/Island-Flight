#pragma once

#include "Light.h"

namespace nsK2EngineLow {
	class ModelRender;
	class RenderContext;

	class RenderingEngine
	{
	public:

		/// <summary>
		/// ���C�g�̏������B
		/// </summary>
		void Init();

		/// <summary>
		/// ���f�������_�[�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="modelRender"></param>
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.emplace_back(modelRender);
		}

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
		void SetDerectionLight(Vector3 direction, Vector3 color)
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

	private:

		/////////////////////////////////////////////////////////////
		//�`��̊֐�
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// ���f����`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void ModelRendering(RenderContext& rc);


		std::vector<ModelRender*>		m_modelList;	//���f�����X�g
		SceneLight						m_sceneLight;	//�V�[�����C�g
	};

}

