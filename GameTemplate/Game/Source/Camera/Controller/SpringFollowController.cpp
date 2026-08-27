#include "Source/stdafx.h"
#include "Source/Camera/Controller/SpringFollowController.h"
namespace
{
	static constexpr float DAMPING_RATE = 0.8f * 2;	//減衰率
	static constexpr float DAMPING_C = 5.0f;	//減衰係数
}

void SpringFollowController::OnEnter()
{
	InitCameraState();
}

void SpringFollowController::InitCameraState()
{

	//ターゲット
	Vector3 targetPositon = Vector3::Zero;
	m_targetSnapshot.GetPosition(targetPositon);
	m_cameraState.targetPos = targetPositon;

	//ポジション
	m_cameraState.pos = targetPositon + m_localCameraOffsetPos;

	//アップ
	m_cameraState.up = g_camera3D->GetUp();
}

Vector3 SpringFollowController::CalcSpringMove(
	float dampingC,
	float dampingRate,
	const TargetSnapshot& snap,
	Vector3 offsetPos
)
{
	//デルタを取得
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	//ターゲットのポジションを取得。
	Vector3 TargetPos = Vector3::One;
	snap.GetPosition(TargetPos);

	//ターゲットの速度を取得。
	Vector3 targetVel = Vector3::One;
	snap.GetVelocity(targetVel);

	//カメラの速度
	Vector3 camVel = m_cameraState.velocity;

	Vector3 camPos = m_cameraState.pos;

	Vector3 targetPosWithOffset = offsetPos + TargetPos;

	Vector3 distance;
	distance = targetPosWithOffset - camPos;

	//ばね定数（ばねの強さ）
	float SpringK = std::pow(dampingC / (2 * dampingRate), 2);

	//相対的な速度（カメラから見たターゲットの速度）
	Vector3 relVelocity = targetVel - camVel;

	//加速度
	//ばねだから自分のポジションから
	//目的のベクトルがそのまま加速度
	Vector3 SpringAccel = distance;

	//ばねの強さにおうじて加速度をスケールする。
	SpringAccel *= SpringK;

	//減衰をさせる
	SpringAccel += relVelocity * dampingC;

	//加速度を積分して速度を求める
	camVel += SpringAccel * deltaTime;

	return camVel;
}

Vector3 SpringFollowController::CalcWoldeCameraOffsetPos(Quaternion orientation, float dt)
{
	//ターゲットの姿勢をカメラのオフセットに適用する。
	Vector3 woldeCameraOffsetPos = m_localCameraOffsetPos;
	orientation.Apply(woldeCameraOffsetPos);

	return woldeCameraOffsetPos;
}

void SpringFollowController::UpdateState(const TargetSnapshot& snap)
{

	if (!snap.GetValid())return;
	//デルタ
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	//ターゲットの速度
	Vector3 targetVel = Vector3::Zero;
	snap.GetVelocity(targetVel);

	//ターゲットの姿勢
	Quaternion orientation = Quaternion::Identity;
	snap.GetRotation(orientation);

	//カメラのポジションのオフセットを求める。（ワールド座標）
	Vector3 woldeCamOffsetPos = CalcWoldeCameraOffsetPos(orientation, deltaTime);

	//カメラの速度
	m_cameraState.velocity = CalcSpringMove(DAMPING_C, DAMPING_RATE, snap, woldeCamOffsetPos);

	//速度を積分
	//変位を足す。
	m_cameraState.pos += m_cameraState.velocity * deltaTime;

	//右スティックの入力に応じてカメラを回転させる。
	{
		Vector3 targetPos = Vector3::Zero;
		snap.GetPosition(targetPos);

		//回転の差分を求める。
		Quaternion qOrbitDelta = MakeOrbitQuaternionByRightStick(deltaTime, orientation);
		//カメラから見たターゲットの位置を回転させる。
		Vector3 cameraRelativePos = m_cameraState.pos - targetPos;
		qOrbitDelta.Apply(cameraRelativePos);
		//回転させた位置をカメラの新しい位置とする。
		m_cameraState.pos = targetPos + cameraRelativePos;
	}

	Vector3 targetPositon = Vector3::One;
	snap.GetPosition(targetPositon);
	m_cameraState.targetPos = targetPositon;

	m_cameraState.up = g_camera3D->GetUp();



}

void SpringFollowController::OnTargetWarped(const TargetSnapshot& snap)
{
	if (!snap.GetValid()) return;

	Vector3 targetPos = Vector3::Zero;
	snap.GetPosition(targetPos);

	Quaternion orientation = Quaternion::Identity;
	snap.GetRotation(orientation);

	//デルタ
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	// ワールドのオフセット
	Vector3 worldOffset = CalcWoldeCameraOffsetPos(orientation, deltaTime);

	//ばねの内部状態をリセット
	m_cameraState.pos = targetPos + worldOffset;
	m_cameraState.targetPos = targetPos;

	Vector3 targetVel = Vector3::Zero;
	snap.GetVelocity(targetVel);
	m_cameraState.velocity = targetVel;

	m_cameraState.up = g_camera3D->GetUp();
}

Quaternion SpringFollowController::MakeOrbitQuaternionByRightStick(float dt, Quaternion targetOrientation)
{
	//右スティックの入力を取得
	float rightStickXInput = g_pad[0]->GetRStickXF();
	float rightStickYInput = g_pad[0]->GetRStickYF();

	const float horizontalSpeed = 0.5f;
	const float verticalSpeed = 0.5f;

	//右スティックの入力に応じてカメラの回転角を更新
	m_orbitHorizontalAngle = -rightStickXInput * horizontalSpeed * dt;
	m_orbitVerticalAngle = rightStickYInput * verticalSpeed * dt;

	////回転角をクランプ
	//m_orbitVerticalAngle = std::clamp(m_orbitVerticalAngle, -XM_PIDIV2 + 0.1f, XM_PIDIV2 - 0.1f);

	//横回転と縦回転のクォータニオン
	Quaternion qHorizontal;
	Quaternion qVertical;
	//ターゲットの姿勢をカメラの回転に適用するために、ワールド座標でのローカルの軸を求める。
	Vector3 upAxisWorld = Vector3::Up;
	Vector3 rightAxisWorld = Vector3::Right;
	targetOrientation.Apply(upAxisWorld);
	targetOrientation.Apply(rightAxisWorld);
	//右スティックの入力に応じて、ワールド座標でのローカルの軸を回転軸としてクォータニオンを作成
	qHorizontal.SetRotation(upAxisWorld, m_orbitHorizontalAngle);
	qVertical.SetRotation(rightAxisWorld, m_orbitVerticalAngle);

	//横回転と縦回転のクォータニオンを掛け合わせて、最終的なカメラの回転クォータニオンを作成
	Quaternion qOrbit = qVertical *qHorizontal;

	return qOrbit;

}
