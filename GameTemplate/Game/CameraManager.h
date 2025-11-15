#pragma once
#include"SceneStateContext.h"
#include "CameraTypes.h"

class ICameraController;
class CameraState;
class CameraManager :public Noncopyable
{
public:
	static CameraManager& GetInstance() {
		static CameraManager instance;
		return instance;
	}
	bool Start();
	void Update();

public:

	void Inint() {}
	/// <summary>
	/// ターゲット情報を外から設定する。
	/// ターゲットが動く場合は毎フレームセットする。
	/// それをそのままターゲットやポジションとして使うわけではなく
	/// ターゲットを参考にしてコントローラーを動かす。
	/// どう使うか（使わないか）決めるのはコントローラー側。
	/// </summary>
	/// <param name="info"></param>
	void SetTargetInfo(const TargetSnapshot& info);

	void ChangeController(CameraControllerType type);

	CameraState GetCameraState() const;

private:
	CameraManager();
	~CameraManager();
private:
	TargetSnapshot m_targetSnapshot;	//ターゲットの情報
	CameraState m_currentState;
	CameraState m_prevState;			// ブレンド用
	ICameraController* m_activeController;
};

