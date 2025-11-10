#pragma once

/// <summary>
/// ƒCƒ“ƒQ[ƒ€scene‚Ì‹¤’Ê‚·‚é•Ï”‚Ì\‘¢‘Ì
/// </summary>
class Aircraft;
class CoinManager;
class ScoreManager;
struct InGameContext
{
	Aircraft* aircraft = nullptr;
	CoinManager* coinManager = nullptr;
	ScoreManager* scoreManager = nullptr;
};
