#include "stdafx.h"
#include "system/system.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "UIManager.h"

// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	// k2EngineLowの初期化。
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });

	// デバッグワイヤーフレーム描画を有効にする。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	GameManager* gameManager = NewGO<GameManager>(0);

	// カメラの視野角を設定。
	g_camera3D->SetViewAngle(0.5);

	g_soundEngine->ResistWaveFileBank(static_cast<int>(SoundID::enTitleBGM), "Assets/sound/BGM/TitleBGM.wav");
	g_soundEngine->ResistWaveFileBank(static_cast<int>(SoundID::enGamePlayBGM), "Assets/sound/BGM/GamePlayBGM.wav");
	g_soundEngine->ResistWaveFileBank(static_cast<int>(SoundID::enCoinSE), "Assets/sound/SE/CoinSE.wav");
	g_soundEngine->ResistWaveFileBank(static_cast<int>(SoundID::enCoinGetSE), "Assets/sound/SE/CoinGetSE.wav");
	g_soundEngine->ResistWaveFileBank(static_cast<int>(SoundID::enCountUpSE), "Assets/sound/SE/CountUp.wav");
	g_soundEngine->ResistWaveFileBank(static_cast<int>(SoundID::enResultEnterSE), "Assets/sound/SE/ResultEnter.wav");

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		g_soundEngine->SetListenerPosition(g_camera3D->GetPosition());
		g_soundEngine->SetListenerFront(g_camera3D->GetForward());
		g_soundEngine->SetListenerUp(g_camera3D->GetUp());
		g_soundEngine->Set3DSoundDistanceScale(0.002f);

		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();

		UIManager::GetInstance().Update();
		CameraManager::GetInstance().Update();
		// ゲームオブジェクトマネージャーの更新処理を呼び出す。
		g_k2EngineLow->ExecuteUpdate();

		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_k2EngineLow->ExecuteRender();

		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}

