#pragma once

namespace nsK2EngineLow {

	/// <summary>
	/// ディレクションライト構造体
	/// </summary>
	struct DirectionalLight
	{
		Vector3 lightDirection;		//ライトの方向
		float pad0;					//パティング
		Vector3 color;				//ライトのカラー
		float pad1;					//パティング
	};

	/// <summary>
	/// ライト構造体
	/// </summary>
	struct Light
	{
		DirectionalLight	directionalLight;		//ディレクションライト
		Vector3				cameraEyePos;			//カメラの座標
		float				pad;					//パティング
		Vector3             ambientColor;           //アンビエントカラー
		float				pad1;					//パティング
		Matrix mLVP;//ライトビュー投影行列。
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
		///ディレクションライトの関数
		////////////////////////////////////////////////////////////////////// 
		/// <summary>
		/// ディレクションライトの設定
		/// </summary>
		/// <param name="direction">ライトの方向</param>
		/// <param name="color">ライトの色</param>
		void SetDirectionLight(Vector3 direction, Vector3 color)
		{
			SetDirLightDirection(direction);
			SetDirLightColor(color);
		}

		/// <summary>
		/// ディレクションライトの光の方向を設定する
		/// </summary>
		/// <param name="direction">方向</param>
		void SetDirLightDirection(Vector3 direction)
		{
			direction.Normalize();
			m_light.directionalLight.lightDirection = direction;
		}

		/// <summary>
		/// ディレクションライトの光の色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetDirLightColor(Vector3 color)
		{
			m_light.directionalLight.color = color;
		}

		/// <summary>
		/// ディレクションライトの光の方向を取得する
		/// </summary>
		/// <returns>光の方向</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_light.directionalLight.lightDirection;
		}

		/// <summary>
		/// ディレクションライトの光の色を取得する
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetDirLigColor() const
		{
			return m_light.directionalLight.color;
		}

		//////////////////////////////////////////////////////////////////////
		///アンビエントライトの関数
		////////////////////////////////////////////////////////////////////// 

		/// <summary>
		/// アンビエントライトを設定する
		/// </summary>
		/// <param name="ambientColor"></param>
		void SetAmbientLight(Vector3 ambientColor)
		{
			m_light.ambientColor= ambientColor;
		}

		//////////////////////////////////////////////////////////////////////
		///ライトビュープロジェクション行列の関数
		////////////////////////////////////////////////////////////////////// 

		/// <summary>
		/// ライトビュープロジェクション行列を設定する
		/// </summary>
		/// <param name="ambientColor"></param>
		void SetLightLVP(Matrix lvp)
		{
			m_light.mLVP = lvp;
		}
		


		////////////////////////////////////////////////////////
		///カメラの位置の関数
		////////////////////////////////////////////////////////

		/// <summary>
		/// カメラの位置を設定する
		/// </summary>
		/// <param name="eyePos"></param>
		void SetEyePos(Vector3 eyePos)
		{
			m_light.cameraEyePos = eyePos;
		}

		////////////////////////////////////////////////////////
		///ライトカメラの関数
		////////////////////////////////////////////////////////

		/// <summary>
		/// ライトカメラの設定。
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
		Camera 					       m_lightCamera;   //ライトカメラ
	};
}

