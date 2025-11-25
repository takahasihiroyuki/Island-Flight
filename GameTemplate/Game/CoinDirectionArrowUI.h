#pragma once
namespace
{
	static constexpr size_t MAX_ARROW_COUNT = 3;
}

#include"UIScreen.h"
class CoinManager;
class Aircraft;
class CoinDirectionArrowUI :public UIScreen
{
public:
	CoinDirectionArrowUI(CoinManager* coinManager, Aircraft* player)
		:m_coinManager(coinManager), m_player(player)
	{}
	~CoinDirectionArrowUI() {};
	void Update()override;
	void Render(RenderContext& rc)override;
	void Init();
	void Open();
	void Close();


private:
	/// <summary>
	/// コインのある方向から矢印の位置を計算する。
	/// </summary>
	/// <param name="playerToCoinDir">コインのある方向</param>
	/// <param name="radius"></param>
	/// <returns></returns>
	Vector3 CalcArrowPosOffsetFromDir(const Vector3& playerToCoinDir, float radius);

	Quaternion CalcArrowRotationFromDir(const Vector3& playerToCoinDir);

private:
	CoinManager* m_coinManager = nullptr;
	Aircraft* m_player = nullptr;
	float m_arrowMaxDistance = 10000.0f;// 矢印が表示される最大距離
	float m_arrowPosSphereRadius = 300.0f;// 矢印の位置を決める球の半径
	std::array<ModelRender, MAX_ARROW_COUNT> m_arrows;

};

