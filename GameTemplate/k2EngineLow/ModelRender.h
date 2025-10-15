#pragma once
#include "Light.h"
#include "GraphicsEnums.h"

namespace nsK2EngineLow {
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		/// <summary>
		/// �ʏ�`��p�̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		/// <param name="isShadowReciever">�e���󂯂鑤��</param>
		/// <param name="isFowardRender">�t�H���[�h�����_�����O�ŕ`�悷�邩</param>
		/// <param name="disableLayer">���˂ŉf�荞�܂Ȃ����C���[</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationCrips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReciever = true,
			bool isFowardRender = false,
			ReflectLayer disableLayer = ReflectLayer::enNone
		);

		/// <summary>
		/// �C�`��p�̏�����
		/// </summary>
		/// <param name="initData"></param>
		/// <param name="tkmFilePath">�t�@�C���p�X</param>
		void InitOcean(ModelInitData& initData);

		/// <summary>
		/// �X�J�C�L���[�u�����Ƃ��Ɏg�p�B
		/// </summary>
		/// <param name="initData"></param>
		void InitSkyCubeModel(ModelInitData& initData);

		void InitSkyCubeReflectionModel(ModelInitData& initData);

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();

		void Draw(RenderContext& rc);

		void PlaneDraw();

		/// <summary>
		/// �f�B�t�@�[�h�p�̃��f���̕`��
		/// </summary>
		/// <param name="rc"></param>
		void OnDraw(RenderContext& rc)
		{
			if (m_isFowardRender) {
				m_frowardRenderModel.Draw(rc);
				return;
			}
			else {
				m_renderToGBufferModel.Draw(rc);

			}
		}

		/// <summary>
		/// ���f����`�悷��(RenderingEngine�ŕ`�悷��Ƃ��ɌĂяo��)�B
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderModel(RenderContext& rc)
		{
			m_model.Draw(rc);
		}

		/// <summary>
		/// �V���h�E�}�b�v�̕`�������i�V���h�E�N���X�ŌĂяo���j�B
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="came"></param>
		void OnRenderShadowMap(RenderContext& rc, Camera& came);

		void OnRenderReflectionMap(RenderContext& rc, Camera& came);

		/// <summary>
		/// �s���ݒ�B
		/// </summary>
		/// <param name="mairix">�s��B</param>
		void SetWorldMatrix(const Matrix& matrix);

		/// <summary>
		/// ���W�A��]�A�g������ׂĐݒ�
		/// </summary>
		/// <param name="pos">���W�̐ݒ�</param>
		/// <param name="rot">��]�̐ݒ�</param>
		/// <param name="scale">�g��̐ݒ�</param>
		void SetTransform(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rot);
			SetScale(scale);
		}

		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="position">Vector3�̍��W</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="x">X��</param>
		/// <param name="y">Y��</param>
		/// <param name="z">Z��</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}
		/// <summary>
		/// ���W�����Z
		/// </summary>
		/// <param name="addPosition"></param>
		void AddPosition(Vector3 addPosition)
		{
			m_position += addPosition;
		}


		/// <summary>
		/// ��]�̐ݒ�
		/// </summary>
		/// <param name="rotation">��]</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// �傫���̐ݒ�
		/// </summary>
		/// <param name="scale">�傫��</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// �傫���̐ݒ�
		/// </summary>
		/// <param name="x">X��</param>
		/// <param name="y">Y��</param>
		/// <param name="z">Z��</param>
		void SetScale(float x, float y, float z)
		{
			SetScale({ x,y,z });
		};

		/// <summary>
		/// �傫���̐ݒ�(�S�Ă̎�����)
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(float scale)
		{
			SetScale({ scale,scale,scale });
		}

		/// <summary>
		/// �e�탂�f���̃��[���h�s����X�V����B
		/// </summary>
		void UpdateWorldMatrixInModes();

		Matrix GetWorldMatrix() const
		{
			return m_model.GetWorldMatrix();
		}

		const Model& GetModel() const
		{
			return m_model;
		}

		const Model& GetRenderToGBufferModel()const 
		{
			return m_renderToGBufferModel;
		}

		/// <summary>
		/// �A�j���[�V�����Đ��B
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
		/// <param name="interpolateTime">�⊮����(�P�ʁF�b�B)</param>
		/// <param name="animReset">�A�j���[�V�������ŏ�����ɂ��邩�B�����A�j���[�V�������瓯���A�j���[�V�����ɑJ�ڂ���Ƃ��Ɏg��</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f, bool animReset = false)
		{
			m_animation.Play(animNo, interpolateTime, animReset);
		}

		/// <summary>
		/// �A�j���[�V�����̍Đ����H
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// �A�j���[�V�����Đ��̑��x��ݒ肷��B
		/// </summary>
		/// <param name="animationSpeed">���l�̕������{�ɂ���B</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		void ChangeAlbedoMap(const char* filePath, Texture& albedoMap)
		{
			m_model.ChangeAlbedoMap(filePath, albedoMap);
			m_renderToGBufferModel.ChangeAlbedoMap(filePath, albedoMap);
			m_shadowModel.ChangeAlbedoMap(filePath, albedoMap);
		}


	private:

		/// <summary>
		/// �X�P���g���̏������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// �A�j���[�V�����̏������B
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		void InitAnimation(
			AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		);

		/// <summary>
		/// GBuffer�`�惂�f���̏�����
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		/// <param name="isShadowReciever"></param>
		void InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever);

		/// <summary>
		/// �A�̃��f��������������B
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitShadowModel(const char* filePath, EnModelUpAxis enModelUpAxis);

		void InitReflectionModel(const char* filePath, EnModelUpAxis enModelUpAxis, ReflectLayer layer);

		void InitSkyCubeReflectionModel(const char* filePath, EnModelUpAxis enModelUpAxis, ReflectLayer layer);





	private:
		Skeleton						m_skeleton;									//�X�P���g��
		AnimationClip* m_animationClips = nullptr;					            //�A�j���[�V�����N���b�v�B
		int								m_numAnimationClips = 0;					//�A�j���[�V�����N���b�v�̐��B
		Animation						m_animation;								//�A�j���[�V�����B
		float							m_animationSpeed = 1.0f;					//�A�j���[�V�����̑��x�B

		Vector3							m_position = Vector3::Zero;				    //���W
		Vector3							m_scale = Vector3::One;					    //�傫��
		Quaternion						m_rotation = Quaternion::Identity;		    //��]

		Model							m_model;								    //���f���N���X
		Model							m_frowardRenderModel;					    // �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
		ModelInitData					m_modelInitData;						    //���f�������������邽�߂̏���ݒ肷��N���X


		bool							m_isFowardRender = false;					//�t�H���[�h�����_�����O�ŕ`�悷�邩
		bool							m_setWorldMatrix = false;			        // ���[���h�s�񂪊O������ݒ肳��Ă���
		bool							m_isInit = false;					        //������������
		std::map<ReflectLayer, bool>	m_enableReflection;   // ���˂ŉf�荞�ނ��ǂ���
		SceneLight  					m_sceneLight;                               //�V�[�����C�g

		Model							m_renderToGBufferModel;	                    // RenderToGBuffer�ŕ`�悳��郂�f��
		Model							m_shadowModel;							    //�e�`��p���f��
		std::map<ReflectLayer, Model>	m_ReflectionModel;                          //���˃}�b�v�`��p���f��
		bool							m_isSkyCube = false;						// �X�J�C�L���[�u���f�����ǂ���
	};
}

