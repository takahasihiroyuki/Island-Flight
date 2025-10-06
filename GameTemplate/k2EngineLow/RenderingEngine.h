#pragma once

#include "Light.h"
#include "PostEffect.h"
#include "Shadow.h"
#include "PlaneReflectionPass.h"
#include "GraphicsEnums.h"

namespace nsK2EngineLow {
	class ModelRender;
	class RenderContext;

	class RenderingEngine
	{
	public:
		RenderTarget m_mainRenderTarget;						//���C�������_�����O�^�[�Q�b�g

		RenderingEngine();
		~RenderingEngine();
		void Update();
		void Init();

		void InitGBuffer();
		void InitMainRenderTarget();
		void InitCopyToframeBufferSprite();
		void InitDefferedLightingSprite();

		void ForwardRendering(RenderContext& rc);

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
			m_sceneLight.SetDirectionLight(direction, color);
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
		/// ���X�g�Ƀf�B�t�@�[�h�p�̃��f�������_�[��ǉ�����B
		/// </summary>
		/// <param name="modelRender"></param>
		void AddDeferredModelList(ModelRender* modelRender)
		{
			m_deferredModelList.push_back(modelRender);
		}

		void AddForwardModelList(ModelRender* modelRender)
		{
			m_forwardModelList.push_back(modelRender);
		}

		void AddreflectedModelList(ModelRender* modelRender, ReflectLayer reflectLayer)
		{
			for (auto it = m_reflectedModelList.begin(); it != m_reflectedModelList.end(); ++it) {
				auto& list = it->second;
				list.push_back(modelRender);
			}
		}

		/// <summary>
		/// ���C�g�J�����̎擾
		/// </summary>
		/// <returns></returns>
		Camera& GetLightCamera()
		{
			return m_sceneLight.GetLightCamera();
		}

		RenderTarget& GetPlaneReflectionRenderTarget(ReflectLayer layer)
		{
			return m_planeReflectionPass[layer].GetPlanarReflectionTarget();
		}

		void SetReflectPlane(Plane& plane, ReflectLayer layer)
		{
			m_reflectPlane.insert_or_assign(layer, plane);
		}

		Matrix GetReflectViewProjectionMatrix(ReflectLayer layer) {
			return m_planeReflectionPass[layer].GetReflectViewProjectionMatrix();
		}


		Camera& GetReflectCamera(ReflectLayer layer) {
			return m_planeReflectionPass[layer].GetReflectCamera();
		}

		ReflectionModelCB& GetReflectionModelCB(ReflectLayer layer) {
			return m_planeReflectionPass[layer].GetConstantBuffer();
		}

		Vector4 GetReflectPlaneEquation(ReflectLayer layer) {
			auto it = m_reflectPlane.find(layer);
			if (it == m_reflectPlane.end()) {
				return Vector4(0, 1, 0, 0);
			}
			return it->second.GetEquation();
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
			enGBufferAlbedoDepth,      // �A���x�h
			enGBufferNormal,           // �@��
			enGBufferSpecular,         // �X�y�L����
			enGBufferWorldPos,         // ���[���h���W
			enGBufferShadow,           // �V���h�E
			enGBufferNum,              // G-Buffer�̐�

		};



		Sprite m_copyToframeBufferSprite;						//���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		Sprite m_diferredLightingSprite;						//�f�B�t�@�[�h���C�e�B���O�p�̃X�v���C�g

		std::array<RenderTarget, enGBufferNum> m_gBuffer;		//GBuffer�p�̃����_�����O�^�[�Q�b�g�B


		std::vector<ModelRender*>							m_deferredModelList;	   //�f�B�t�@�[�h���f�����X�g
		std::vector<ModelRender*>							m_forwardModelList;		   //�t�H���[�h���f�����X�g
		SceneLight											m_sceneLight;	           //�V�[�����C�g
		PostEffect											m_postEffect;	           //�|�X�g�G�t�F�N�g
		RenderTarget										m_shadowMapRenderTarget;   //�V���h�E�}�b�v�p�����_�����O�^�[�Q�b�g
		Shadow												m_shadow;                  //�V���h�E
		std::map<ReflectLayer, PlaneReflectionPass>			m_planeReflectionPass;     // ���ʔ��˃p�X
		std::map<ReflectLayer, std::vector<ModelRender*>>	m_reflectedModelList;	   //���ʃ��f�����X�g
		std::map<ReflectLayer, Plane>						m_reflectPlane;	           // ���˕���


		// �f�o�b�O�p
		Sprite m_debugReflectionSprite;
		bool   m_showReflectionDebug = true; // �ꎞ�I
	};

}

