#pragma once

struct ISceneStateContext {};

/// <summary>
/// ƒCƒ“ƒQ[ƒ€scene‚Ì‹¤’Ê‚·‚é•Ï”‚Ì\‘¢‘Ì
/// </summary>
class Aircraft;
class CoinManager;
class ScoreManager;
class Stage;
struct InGameContext :ISceneStateContext
{
	Aircraft* aircraft = nullptr;
	CoinManager* coinManager = nullptr;
	ScoreManager* scoreManager = nullptr;
	Stage* stage = nullptr;
};
