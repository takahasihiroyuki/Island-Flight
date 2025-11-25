#include "stdafx.h"
#include "CoinDirectionArrowUI.h"
#include "CoinManager.h"
#include "Aircraft.h"
#include "UIManager.h"

namespace {
	static const Vector3 ARROW_SCALE = Vector3{ 5,5,5 };
}

void CoinDirectionArrowUI::Update()
{
	if (!m_coinManager && !m_player)return;

	// まず全ての矢印を非表示にする
	for (int i = 0; i < MAX_ARROW_COUNT; i++)
	{
		m_arrows[i].SetVisible(false);
	}

	// プレイヤーから見たコインへのベクトルを取得する
	std::vector<Vector3> playerToCoinVecs;
	playerToCoinVecs = m_coinManager->GetPlayerToCoinVecs(m_player->GetPosition(), m_arrowMaxDistance, MAX_ARROW_COUNT);

	// 矢印の数を調整する
	for (int i = 0; i < min(playerToCoinVecs.size(), MAX_ARROW_COUNT); i++)
	{
		//プレイヤーからコインへの方向ベクトル
		Vector3 playerToCoinDir = playerToCoinVecs[i];
		playerToCoinDir.Normalize();

		Vector3 arrow3DposOffset = CalcArrowPosOffsetFromDir(playerToCoinDir, m_arrowPosSphereRadius);
		Vector3 arrow3Dpos = m_player->GetPosition() + arrow3DposOffset;
		Quaternion arrowRot = CalcArrowRotationFromDir(playerToCoinDir);

		m_arrows[i].SetPosition(arrow3Dpos);
		m_arrows[i].SetRotation(arrowRot);
		m_arrows[i].SetScale(ARROW_SCALE);
		m_arrows[i].Update();
		m_arrows[i].SetVisible(true);
	}
}

void CoinDirectionArrowUI::Render(RenderContext& rc)
{
	for (int i = 0; i < MAX_ARROW_COUNT; i++)
	{
		m_arrows[i].Draw(rc);
	}

}

void CoinDirectionArrowUI::Init()
{
	for (int i = 0; i < MAX_ARROW_COUNT; i++)
	{
		m_arrows[i].Init("Assets/UI/arrow/arrow.tkm");
		m_arrows[i].SetScale(ARROW_SCALE);
		m_arrows[i].SetVisible(false);		// 初期状態では非表示にする
		m_arrows[i].Update();
	}


}

void CoinDirectionArrowUI::Open()
{
}

void CoinDirectionArrowUI::Close()
{
	UIManager::GetInstance().UnregisterScreen("coinArrowUI");
}

Vector3 CoinDirectionArrowUI::CalcArrowPosOffsetFromDir(const Vector3& playerToCoinDir, float radius)
{
	Vector3 dir = playerToCoinDir;
	dir.Normalize();
	return dir * radius;
}

Quaternion CoinDirectionArrowUI::CalcArrowRotationFromDir(const Vector3& playerToCoinDir)
{
	//XZ平面への投影の長さ
	float xzProjectionLength =
		sqrtf(playerToCoinDir.x * playerToCoinDir.x +
			playerToCoinDir.z * playerToCoinDir.z);

	// 水平・垂直の回転角度を計算
	float yaw = atan2f(playerToCoinDir.z, playerToCoinDir.x);// 水平方向の回転角度
	float pitch = atan2f(playerToCoinDir.y, xzProjectionLength);// 上下の回転角度

	Quaternion rotYaw, rotPitch;
	// 水平方向回転のQuaternionを計算
	rotYaw.SetRotationY(-yaw);

	//上下方向回転のQuaternionを計算
	Vector3 arrowYawDir = Vector3::Right;
	rotYaw.Apply(arrowYawDir);
	arrowYawDir.y = 0.0f; // Y成分は無視
	arrowYawDir.Normalize();
	// ピッチ回転の軸を計算
	Vector3 axisPith = Cross(arrowYawDir, Vector3::Up);
	axisPith.Normalize();
	rotPitch.SetRotation(axisPith, pitch);

	Quaternion outRot;
	//回転を合成
	outRot = rotPitch * rotYaw;

	return outRot;
}
