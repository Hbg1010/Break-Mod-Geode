#include "../timer/Layers/TimerSettingsLayer.hpp"
#include "../timer/Layers/TimerSettingsButton.hpp"
#include "../timer/Layers/TimerSettingsButton.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(TimerPauseLayer, PauseLayer) {
	void customSetup();
};