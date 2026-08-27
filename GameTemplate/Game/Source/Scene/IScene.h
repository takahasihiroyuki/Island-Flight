#pragma once
#include "Source/Scene/SceneType.h"
class SceneManager;
class IScene :public IGameObject
{
public:
	IScene();
	virtual ~IScene();
	virtual bool Start() override = 0;
	virtual void Update()=0;
	virtual void Render(RenderContext& rc) override = 0;


public:

	virtual void Enter() =0;
	virtual void Exit() =0;

	/// <summary>
	/// 次のシーンをリクエスト
	/// 中で引数に次のシーンを渡す
	/// </summary>
	/// <param name="type"></param>
	/// <returns>sceneを変えたい瞬間にtrueにする</returns>
	virtual bool RequestChangeScene(SceneType& type) = 0;
};

