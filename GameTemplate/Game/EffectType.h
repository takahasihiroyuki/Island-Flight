#pragma once

enum EffectType {
	enCoinGet,
	enSmoke,
	enSpeedLine,
	enSplash,
	enEffectTypeMax
};

static const char16_t* effectPath[enEffectTypeMax] = {
	u"Assets/Effect/GetCoin/GetCoinEffect.efk",
	u"Assets/Effect/Smoke/Smoke.efk",
	u"Assets/Effect/SpeedLines/SpeedLine.efk",
	u"Assets/Effect/Splash/Splash.efk",
};
