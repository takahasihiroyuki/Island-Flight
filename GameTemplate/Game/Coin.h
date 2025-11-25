#pragma once
#include"PlacementObject.h"

// ê∂ê¨ópÇÃê›åvê}
struct CoinDesc {
	Vector3    pos;
	Quaternion rot;
	Vector3    scale;
};

struct CoinDesc;
class Coin : public ItemMeshObject
{
public:
	Coin();
	~Coin();
	void OnStart() override {}
	void OnInit()  override {}
	void OnUpdate() override;
};

