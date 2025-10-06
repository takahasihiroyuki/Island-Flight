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
		RenderTarget m_mainRenderTarget;						//メインレンダリングターゲット

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
			m_sceneLight.SetDirectionLight(direction, color);
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
		/// リストにディファード用のモデルレンダーを追加する。
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
		/// ライトカメラの取得
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
		/// ポストエフェクト
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void PostEffecting(RenderContext& rc);


	private:
		// GBufferに入れるレンダリングターゲットの役割。
		enum EnGBuffer
		{
			enGBufferAlbedoDepth,      // アルベド
			enGBufferNormal,           // 法線
			enGBufferSpecular,         // スペキュラ
			enGBufferWorldPos,         // ワールド座標
			enGBufferShadow,           // シャドウ
			enGBufferNum,              // G-Bufferの数

		};



		Sprite m_copyToframeBufferSprite;						//メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		Sprite m_diferredLightingSprite;						//ディファードライティング用のスプライト

		std::array<RenderTarget, enGBufferNum> m_gBuffer;		//GBuffer用のレンダリングターゲット。


		std::vector<ModelRender*>							m_deferredModelList;	   //ディファードモデルリスト
		std::vector<ModelRender*>							m_forwardModelList;		   //フォワードモデルリスト
		SceneLight											m_sceneLight;	           //シーンライト
		PostEffect											m_postEffect;	           //ポストエフェクト
		RenderTarget										m_shadowMapRenderTarget;   //シャドウマップ用レンダリングターゲット
		Shadow												m_shadow;                  //シャドウ
		std::map<ReflectLayer, PlaneReflectionPass>			m_planeReflectionPass;     // 平面反射パス
		std::map<ReflectLayer, std::vector<ModelRender*>>	m_reflectedModelList;	   //平面モデルリスト
		std::map<ReflectLayer, Plane>						m_reflectPlane;	           // 反射平面


		// デバッグ用
		Sprite m_debugReflectionSprite;
		bool   m_showReflectionDebug = true; // 一時的
	};

}

