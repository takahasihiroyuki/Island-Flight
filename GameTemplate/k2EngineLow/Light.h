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

	};

	class SceneLight
	{
	public:
		void Init();

		Light& GetSceneLight()
		{
			return m_light;
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

	private:
		Light m_light;
	};
}

