#pragma once

class StageMeshObject;
class CoinManager;
class Stage :public IGameObject
{
public:
	Stage() {
		float debug = 0;
	};
	~Stage() {
		DeleteGO(m_instancingManager);

	};

	bool Start() override;
	void GetCoinManager(CoinManager* coinManager)
	{
		m_coinManager = coinManager;
	}
	void Update() {
	}

	/// <summary>
	/// �t�H�O�̃p�����[�^��ݒ�
	/// </summary>
	/// <param name="collar"></param>
	/// <param name="fogDistanceScale"></param>
	void SetFogParams(Vector3 collar, float fogDistanceScale) {
		g_renderingEngine->SetFogParams(collar, fogDistanceScale);
	}
private:
	CoinManager* m_coinManager;
	InstancingManager* m_instancingManager = nullptr;
	Vector3 m_posOfset = Vector3::Zero;
	Vector3 m_ScaleOfset;

};

