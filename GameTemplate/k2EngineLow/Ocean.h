#pragma once

namespace nsK2EngineLow {
	namespace {
		const Vector3 PLANE_NORMAL = g_vec3Up;	// ���ʂ̖@��
		const Vector3 PLANE_POSITION = g_vec3Zero;// ���ʂ̃|�W�V�����i���ʂ��ʂ�_�j
	}

	class Ocean :public IGameObject
	{
		struct OceanConstantBuffer
		{
			Matrix ReflectionCameraVP; // ���˗p�J�����r���[���e�s��
			Light light;
		};


	public:
		Ocean();
		~Ocean();
		bool Start()override;
		void Init();
		void InitOceanSprite();
		void Update()override;
		void Render(RenderContext& rc)override;

	private:
		/// <summary>
/// �ʒu��ݒ�
/// </summary>
/// <param name="pos">���W</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
			m_isDirty = true;
		}
		/// <summary>
		/// �傫����ݒ�
		/// </summary>
		/// <param name="scale">�g�嗦</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
			m_isDirty = true;
		}
		/// <summary>
		/// �傫����ݒ�
		/// </summary>
		/// <param name="scale">��]</param>
		void SetScale(const float scale)
		{
			m_scale = g_vec3One;
			m_scale.Scale(scale);
			m_isDirty = true;
		}
		/// <summary>
		/// ��]��ݒ�
		/// </summary>
		/// <param name="rotation"></param>
		void SetRotation(Quaternion rotation)
		{
			m_rotation = rotation;
			m_isDirty = true;
		}

		void SetConstatntBuffer(const Matrix& vpMatrix, const Light& inLight,Vector3 cameraPos) {
			m_constantBuffer.ReflectionCameraVP = vpMatrix;
			m_constantBuffer.light = inLight;
			m_constantBuffer.light.cameraEyePos = cameraPos;
		};



	private:
		ModelRender m_modelRender;
		Vector3 m_position = g_vec3Zero;
		Vector3 m_scale = g_vec3One * 1000.0f;
		Quaternion m_rotation = Quaternion::Identity;
		bool m_isDirty = false;
		RenderTarget* m_reflectionRenderTarget;
		Light m_light;
		Plane m_plane = Plane(PLANE_NORMAL, PLANE_POSITION);
		OceanConstantBuffer m_constantBuffer;
	};
}

