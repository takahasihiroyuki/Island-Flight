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
		/// GBufferにレンダリングする。
		/// </summary>
		/// <param name="rc"></param>
		void RenderToGBuffer(RenderContext& rc);

		/// <summary>
		/// ディファードレンダリングを行う。
		/// </summary>
		/// <param name="rc"></param>
		void DeferredLighting(RenderContext& rc);

		/// <summary>
		/// メインレンダーターゲットをフレームバッファにコピーする。
		/// </summary>
		/// <param name="rc"></param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

		/// <summary>
		/// 描画したオブジェクトをクリアする。
		/// </summary>
		void ObjectClear();


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
		void SetDirectionLight(Vector3 direction, Vector3 color)
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

		void SetAmbientLight(Vector3 ambientColor)
		{
			m_sceneLight.SetAmbientLight(ambientColor);
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

		/// <summary>
		/// リストにモデルレンダーを追加する。
		/// </summary>
		/// <param name="modelRender"></param>
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.push_back(modelRender);
		}

	private:
		/// <summary>
		/// ポストエフェクト
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void PostEffecting(RenderContext& rc);


	private:
		// GBufferに入れるレンダリングターゲットの役割。
		enum EnGBuffer
		{
			enGBufferAlbedo,           // アルベド
			enGBufferNormal,           // 法線
			enGBufferSpecular,         // スペキュラ
			enGBufferNum,              // G-Bufferの数
		};

		Sprite m_copyToframeBufferSprite;				//メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		Sprite m_diferredLightingSprite;				//ディファードライティング用のスプライト

		std::array<RenderTarget, enGBufferNum> m_gBuffer;			//GBuffer用のレンダリングターゲット。
		RenderTarget m_mainRenderTarget;				//メインレンダリングターゲット


		std::vector<ModelRender*>		m_modelList;	//モデルリスト
		SceneLight						m_sceneLight;	//シーンライト
		PostEffect						m_postEffect;	//ポストエフェクト
	};

}

