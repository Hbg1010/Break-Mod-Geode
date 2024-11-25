#include "../timer/Layers/TimerSettingsLayer.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(TimerPauseLayer, PauseLayer) {
	void customSetup();
	void onTimerSettings(CCObject* sender);
};