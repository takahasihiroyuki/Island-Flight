#pragma once
#include"PlacementObject.h"

// ê∂ê¨ópÇÃê›åvê}
struct CoinDesc {
	Vector3    pos;
	Quaternion rot;
	Vector3    scale;
};

struct CoinDesc;
class Coin : public PlacementObject
{
public:
	Coin();
	~Coin();

	void Initialize(const CoinDesc& coinDesc)
	{
		m_modelRender.Init("Assets/modelData/coin.tkm");
		m_modelRender.SetPosition(coinDesc.pos);
		m_modelRender.SetRotation(coinDesc.rot);
		m_modelRender.SetScale(coinDesc.scale);
		m_modelRender.Update();
	}

};

