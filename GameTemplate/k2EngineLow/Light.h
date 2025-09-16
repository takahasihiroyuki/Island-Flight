#pragma once

namespace nsK2EngineLow {

	/// <summary>
	/// �f�B���N�V�������C�g�\����
	/// </summary>
	struct DirectionalLight
	{
		Vector3 lightDirection;		//���C�g�̕���
		float pad0;					//�p�e�B���O
		Vector3 color;				//���C�g�̃J���[
		float pad1;					//�p�e�B���O
	};

	/// <summary>
	/// ���C�g�\����
	/// </summary>
	struct Light
	{
		DirectionalLight	directionalLight;		//�f�B���N�V�������C�g
		Vector3				cameraEyePos;			//�J�����̍��W
		float				pad;					//�p�e�B���O
		Vector3             ambientColor;           //�A���r�G���g�J���[
		float				pad1;					//�p�e�B���O
		Matrix mLVP;//���C�g�r���[���e�s��B
	};

	class SceneLight
	{
	public:
		void Init();
		void LightCameraUpdate();

		Light& GetSceneLight()
		{
			return m_light;
		}

		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}

		//////////////////////////////////////////////////////////////////////
		///�f�B���N�V�������C�g�̊֐�
		////////////////////////////////////////////////////////////////////// 
		/// <summary>
		/// �f�B���N�V�������C�g�̐ݒ�
		/// </summary>
		/// <param name="direction">���C�g�̕���</param>
		/// <param name="color">���C�g�̐F</param>
		void SetDirectionLight(Vector3 direction, Vector3 color)
		{
			SetDirLightDirection(direction);
			SetDirLightColor(color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		void SetDirLightDirection(Vector3 direction)
		{
			direction.Normalize();
			m_light.directionalLight.lightDirection = direction;
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetDirLightColor(Vector3 color)
		{
			m_light.directionalLight.color = color;
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_light.directionalLight.lightDirection;
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetDirLigColor() const
		{
			return m_light.directionalLight.color;
		}

		//////////////////////////////////////////////////////////////////////
		///�A���r�G���g���C�g�̊֐�
		////////////////////////////////////////////////////////////////////// 

		/// <summary>
		/// �A���r�G���g���C�g��ݒ肷��
		/// </summary>
		/// <param name="ambientColor"></param>
		void SetAmbientLight(Vector3 ambientColor)
		{
			m_light.ambientColor= ambientColor;
		}

		//////////////////////////////////////////////////////////////////////
		///���C�g�r���[�v���W�F�N�V�����s��̊֐�
		////////////////////////////////////////////////////////////////////// 

		/// <summary>
		/// ���C�g�r���[�v���W�F�N�V�����s���ݒ肷��
		/// </summary>
		/// <param name="ambientColor"></param>
		void SetLightLVP(Matrix lvp)
		{
			m_light.mLVP = lvp;
		}
		


		////////////////////////////////////////////////////////
		///�J�����̈ʒu�̊֐�
		////////////////////////////////////////////////////////

		/// <summary>
		/// �J�����̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="eyePos"></param>
		void SetEyePos(Vector3 eyePos)
		{
			m_light.cameraEyePos = eyePos;
		}

		////////////////////////////////////////////////////////
		///���C�g�J�����̊֐�
		////////////////////////////////////////////////////////

		/// <summary>
		/// ���C�g�J�����̐ݒ�B
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="target"></param>
		void SetLightCamera(Vector3 pos, Vector3 target)
		{
			m_lightCamera.SetPosition(pos);
			target.Normalize();
			m_lightCamera.SetTarget(target);
		}

	private:
		Light m_light;
		Camera 					       m_lightCamera;   //���C�g�J����
	};
}

