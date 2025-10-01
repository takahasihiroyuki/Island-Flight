#pragma once

namespace nsK2EngineLow {
	namespace {
		const Vector3 PLANE_NORMAL = g_vec3Up;	// 平面の法線
		const Vector3 PLANE_POSITION = g_vec3Zero;// 平面のポジション（平面が通る点）
	}

	class Ocean :public IGameObject
	{
		struct OceanConstantBuffer
		{
			Matrix ReflectionCameraVP; // 反射用カメラビュー投影行列
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
/// 位置を設定
/// </summary>
/// <param name="pos">座標</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
			m_isDirty = true;
		}
		/// <summary>
		/// 大きさを設定
		/// </summary>
		/// <param name="scale">拡大率</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
			m_isDirty = true;
		}
		/// <summary>
		/// 大きさを設定
		/// </summary>
		/// <param name="scale">回転</param>
		void SetScale(const float scale)
		{
			m_scale = g_vec3One;
			m_scale.Scale(scale);
			m_isDirty = true;
		}
		/// <summary>
		/// 回転を設定
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

