#pragma once
#include "SceneType.h"
class SceneManager;
class IScene :public IGameObject
{
public:
	IScene();
	virtual ~IScene();
	virtual bool Start() override = 0;
	virtual void Update()=0;
	virtual void Render(RenderContext& rc) override = 0;
	virtual void Init()=0;


public:

	/// <summary>
	/// このシーンに入ったときに呼ばれる
	/// 子で何も書くことがないことがあるかもしれないので
	/// デフォルト実装を用意しておく
	/// </summary>
	void Enter() {};

	/// <summary>
	/// このシーンから出るときに呼ばれる
	/// 子で何も書くことがないことがあるかもしれないので
	/// デフォルト実装を用意しておく
	/// </summary>
	void Exit() {};

	/// <summary>
	/// 次のシーンをリクエスト
	/// 中で引数に次のシーンを渡す
	/// </summary>
	/// <param name="type"></param>
	/// <returns>sceneを変えたい瞬間にtrueにする</returns>
	virtual bool RequestChangeScene(SceneType& type) = 0;
};

