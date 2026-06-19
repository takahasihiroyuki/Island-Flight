#pragma once

enum EffectType {
	enCoinGet,
	enSmoke,
	enSpeedLine,
	enSplash,
	enCoinBoostPickup,
	enScoreBoostAuraEffect,
	enAddLimitCollectEffect,
	enScoreBoostPickup,
	enEffectTypeMax,
};

static const char16_t* effectPath[enEffectTypeMax] = {
	u"Assets/Effect/GetCoin/GetCoinEffect.efk",
	u"Assets/Effect/Smoke/Smoke.efk",
	u"Assets/Effect/SpeedLines/SpeedLine.efk",
	u"Assets/Effect/Splash/Splash.efk",
	u"Assets/Effect/CoinBoostPickupEffect/CoinBoostPickupEffect.efk",
	u"Assets/Effect/ScoreBoost/ScoreBoostAuraEffect.efk",
	u"Assets/Effect/AddLimitCollectEffect/AddLimitCollectEffect.efk",
	u"Assets/Effect/ScoreBoostPickup/ScoreBoostPickup.efk",
};
