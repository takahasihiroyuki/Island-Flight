#include "stdafx.h"
#include "SpringFollowController.h"
namespace
{
	static constexpr float DAMPING_RATE = 0.8f*2;	//減衰率
	static constexpr float DAMPING_C = 5.0f*2;	//減衰係数
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
	Vector3 targetVel= Vector3::One;
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
	Vector3 relVelocity = targetVel -camVel;

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

Vector3 SpringFollowController::CalcWoldeCameraOffsetPos(Quaternion orientation)
{
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
	Quaternion orientation=Quaternion::Identity;
	snap.GetRotation(orientation);

	//カメラのポジションのオフセットを求める。（ワールド座標）
	Vector3 woldeCamOffsetPos=CalcWoldeCameraOffsetPos(orientation);

	//カメラの速度
	m_cameraState.velocity = CalcSpringMove(DAMPING_C, DAMPING_RATE, snap, woldeCamOffsetPos);

	//速度を積分
	//変位を足す。
	m_cameraState.pos += m_cameraState.velocity * deltaTime;

	Vector3 targetPositon = Vector3::One;
	snap.GetPosition(targetPositon);
	m_cameraState.targetPos = targetPositon;

	m_cameraState.up = g_camera3D->GetUp();



}
