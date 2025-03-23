#include <Geode/modify/EditorPauseLayer.hpp>
#include "../timer/Layers/TimerSettingsLayer.hpp"
#include "../timer/Layers/TimerSettingsButton.hpp"
#include "EditorUITimer.hpp"

using namespace geode::prelude;

class $modify(EditorTimerPause, EditorPauseLayer) {
	bool init(LevelEditorLayer* lvl);
	void onResume(CCObject* sender);
	void onExitEditor(cocos2d::CCObject* sender);
};