#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"
#include "SpriteRender.h"

namespace nsK2EngineLow
{

	namespace
	{
		const Vector3 SCENELIGHT_DHIRECTIONLIGHT_COLOR = Vector3(1.0f, 0.95f, 0.80f);
		const Vector3 SCENELIGHT_DHIRECTIONLIGHT_DIRECTION = Vector3(0.6f, 0.1f, 0.3f);
		const Vector3 SCENELIGHT_AMBIENTLIGHT_COLOR = Vector3(0.43f, 0.47f, 0.50f)*1.8/*Vector3(1.0f, 1.0f, 1.0f)*0.7*/;
		const Vector3 LIGHTCAMERA_POSITION = Vector3(100.0f, 2000.0f, 500.0f);
	}

	RenderingEngine::RenderingEngine()
	{
		for (int i = 0; i < static_cast<int>(ReflectLayer::enNum); ++i) {
			auto layer = static_cast<ReflectLayer>(i);

			m_planeReflectionPass.try_emplace(layer);
			m_reflectedModelList.try_emplace(layer);
			m_reflectPlane.try_emplace(layer, Vector3(0, 1, 0), 0.0f);
		}
	}

	RenderingEngine::~RenderingEngine()
	{

	}

	void RenderingEngine::Update()
	{
		//// 反射カメラの更新
		//for (auto it = m_planeReflectionPass.begin(); it != m_planeReflectionPass.end(); ++it) {
		//	ReflectLayer layer = it->first;     // マップのキー
		//	auto& pass = it->second;
		//	pass.UpdateReflectCamera(m_reflectPlane.at(layer));
		//}

	}

	void RenderingEngine::Init()
	{
		// …既存初期化…
		SpriteInitData sd{};
		sd.m_textures[0] = &m_planeReflectionPass[ReflectLayer::enOcean]
			.GetPlanarReflectionTarget()        // ← PlaneReflectionPass側にGetterを用意
			.GetRenderTargetTexture(); // ← カラーのSRVを返すラッパ
		sd.m_width = 512;   // 好きなサイズ
		sd.m_height = 288;
		sd.m_fxFilePath = "Assets/shader/sprite.fx";
		m_debugReflectionSprite.Init(sd);


		//シャドウマップの初期化。
		m_shadow.Init();

		InitZPrepassRenderTarget();

		InitMainRenderTarget();

		InitGBuffer();

		//ライトの設定。
		m_sceneLight.Init(SCENELIGHT_DHIRECTIONLIGHT_DIRECTION,
			SCENELIGHT_DHIRECTIONLIGHT_COLOR,
			SCENELIGHT_AMBIENTLIGHT_COLOR,
			LIGHTCAMERA_POSITION,
			g_camera3D->GetViewProjectionMatrixInv()
		);

		//ポストエフェクトの初期化。
		m_postEffect.Init(
			m_mainRenderTarget
		);

		InitDefferedLightingSprite();

		InitCopyToframeBufferSprite();

		for (auto it = m_planeReflectionPass.begin(); it != m_planeReflectionPass.end(); ++it) {
			auto& pass = it->second;
			pass.Init();
		}

	}

	void RenderingEngine::InitMainRenderTarget()
	{
		//メインレンダリングターゲットの作成
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitZPrepassRenderTarget()
	{
		float clearColor[] = { 100.0f,100.0f,100.0f,1.0f };
		m_zprepassRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}



	void RenderingEngine::InitCopyToframeBufferSprite()
	{
		//メインレンダリングターゲットの絵をフレームバッファにコピーするためのスプライトを初期化
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_copyToframeBufferSprite.Init(spriteInitData);
	}


	void RenderingEngine::InitGBuffer()
	{		// アルベドカラー用のターゲットを作成
		float clearColor[] = { 0.5f,0.5f,0.5f,1.0f };
		m_gBuffer[enGBufferAlbedoDepth].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		//法線用のターゲットを作成
		m_gBuffer[enGBufferNormal].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

		//スペキュラカラー用のターゲットを作成
		m_gBuffer[enGBufferSpecular].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

	}

	void RenderingEngine::InitDefferedLightingSprite()
	{
		BeginGPUEvent("DefferedLightingSprite");
		//ディファードライティングを行うためのスプライトを初期化
		SpriteInitData spriteInitData;
		spriteInitData.m_width = FRAME_BUFFER_W;
		spriteInitData.m_height = FRAME_BUFFER_H;

		// ディファードライティングで使用するテクスチャを設定
		spriteInitData.m_textures[enGBufferAlbedoDepth] = &m_gBuffer[enGBufferAlbedoDepth].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferNormal] = &m_gBuffer[enGBufferNormal].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferSpecular] = &m_gBuffer[enGBufferSpecular].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferShadow] = &m_shadow.GetShadowTarget().GetRenderTargetTexture();


		spriteInitData.m_fxFilePath = "Assets/shader/deferredLighting.fx";

		spriteInitData.m_expandConstantBuffer = &GetSceneLight().GetLight();
		spriteInitData.m_expandConstantBufferSize = sizeof(GetSceneLight().GetLight());

		// ディファードレンダリング用のスプライトを初期化
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(
			m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
			m_gBuffer[enGBufferAlbedoDepth].GetDSVCpuDescriptorHandle()
		);

		for (auto& renderObj : m_forwardModelList) {
			renderObj->OnDraw(rc);
		}

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		BeginGPUEvent("ZPrepass");

		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& renderObj : m_zprepassModelList) {
			renderObj->OnZPrepass(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
		EndGPUEvent();
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		BeginGPUEvent("RenderToGBuffer");
		// レンダリングターゲットをG-Bufferに変更して書き込む
		RenderTarget* rts[] = {
			&m_gBuffer[enGBufferAlbedoDepth]   // 0番目のレンダリングターゲット
			,&m_gBuffer[enGBufferNormal]   // 1番目のレンダリングターゲット
			,&m_gBuffer[enGBufferSpecular] // 2番目のレンダリングターゲット
		};

		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットを設定
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		// まとめてモデルレンダーを描画
		for (auto& MobjData : m_deferredModelList)
		{
			MobjData->OnDraw(rc);
		}

		// レンダリングターゲットへの書き込み待ち
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);


	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{

		GetSceneLight().SetEyePos(g_camera3D->GetPosition());
		// カメラの逆行列を定数バッファにセット
		GetSceneLight().SetCameraViewProjInv(g_camera3D->GetViewProjectionMatrixInv());

		// レンダリング先をメインレンダリングターゲットにする
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		// G-Bufferの内容を元にしてディファードライティング
		m_diferredLightingSprite.Draw(rc);

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

	}


	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		// フレームバッファにセット
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		m_copyToframeBufferSprite.Draw(rc);

	}

	void RenderingEngine::ObjectClear()
	{
		m_deferredModelList.clear();
		m_forwardModelList.clear();
		m_spriteRenderList.clear();
		m_zprepassModelList.clear();
		for (auto& kv : m_reflectedModelList) {    // レイヤーごとに中身だけ消す
			kv.second.clear();
		}
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		// どのレイヤーに対しても、対応するモデルリストがあれば Execute
		for (auto it = m_planeReflectionPass.begin(); it != m_planeReflectionPass.end(); ++it) {
			ReflectLayer layer = it->first;
			auto& pass = it->second;
			auto& objects = m_reflectedModelList[layer];
			pass.UpdateReflectCamera(m_reflectPlane.at(layer));
			pass.Update();
			pass.Execute(rc, objects);
		}

		ZPrepass(rc);

		//GBufferへのレンダリング
		RenderToGBuffer(rc);

		m_shadow.Execute(rc, m_deferredModelList);


		//ディファードライティング
		DeferredLighting(rc);

		ForwardRendering(rc);

		PostEffecting(rc);

		SpriteRendering(rc);

		//メインレンダリングターゲットの絵をフレームバッファにコピー
		CopyMainRenderTargetToFrameBuffer(rc);

		//描画したオブジェクトをクリアする
		ObjectClear();


		//if (m_showReflectionDebug) {
		//	// フレームバッファをRTV/DSVにセット
		//	g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
		//	// 画面左下などに出す（Spriteに位置APIがなければフルスクリーンでも可）
		//	m_debugReflectionSprite.Draw(rc);
		//}
	}

	void RenderingEngine::SpriteRendering(RenderContext& rc)
	{
		for (auto& renderObj : m_spriteRenderList) {
			renderObj->OnDraw(rc);
		}
	}

	void RenderingEngine::PostEffecting(RenderContext& rc)
	{
		m_postEffect.Render(rc, m_mainRenderTarget);
	}

}