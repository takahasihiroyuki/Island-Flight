#pragma once

#include "Light.h"

namespace nsK2EngineLow {
	class ModelRender;
	class RenderContext;

	class RenderingEngine
	{
	public:

		/// <summary>
		/// ライトの初期化。
		/// </summary>
		void Init();

		/// <summary>
		/// モデルレンダーをリストに追加する
		/// </summary>
		/// <param name="modelRender"></param>
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.emplace_back(modelRender);
		}

		/// <summary>
		/// 描画処理を実行。
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// シーンライトの取得。
		/// </summary>
		/// <returns></returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}

		/////////////////////////////////////////////////////
		///ディレクションライトの関数
		/////////////////////////////////////////////////////

		/// <summary>
		/// ディレクションライトの設定
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <param name="direction">ライトの方向</param>
		/// <param name="color">ライトの色</param>
		void SetDerectionLight(Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight( direction, color);
		}

		/// <summary>
		/// ディレクションライトの光の方向を設定する
		/// </summary>
		/// <param name="direction">方向</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_sceneLight.SetDirLightDirection(direction);
		}
		/// <summary>
		/// ディレクションライトの光の色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetDirLightColor(Vector3 color)
		{
			m_sceneLight.SetDirLightColor(color);
		}

		/// <summary>
		/// ディレクションライトの光の方向を取得する
		/// </summary>
		/// <returns>光の方向</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_sceneLight.GetDirLigDirection();
		}
		/// <summary>
		/// ディレクションライトの光の色を取得する
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetDirLigColor() const
		{
			return m_sceneLight.GetDirLigColor();
		}

		/////////////////////////////////////////////////////////////
		//カメラの関数
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// カメラの位置を設定する
		/// </summary>
		/// <param name="eyePos">カメラの座標</param>
		void SetEyePos(Vector3 eyePos)
		{
			m_sceneLight.SetEyePos(eyePos);
		}

	private:

		/////////////////////////////////////////////////////////////
		//描画の関数
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// モデルを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void ModelRendering(RenderContext& rc);


		std::vector<ModelRender*>		m_modelList;	//モデルリスト
		SceneLight						m_sceneLight;	//シーンライト
	};

}

