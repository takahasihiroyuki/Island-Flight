#pragma once
class CameraManager;
class SceneManager;
class GameManager :public IGameObject
{
public:
	GameManager() {};
	~GameManager() {};
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

private:
	SceneManager* m_sceneManager = nullptr;
};

