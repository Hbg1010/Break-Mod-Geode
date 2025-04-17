#include "EditorUITimer.hpp"
#include <Geode/modify/SetupTriggerPopup.hpp>

using namespace geode::prelude;

class $modify(TriggerFix, SetupTriggerPopup) {
    bool init(EffectGameObject* trigger, CCArray* triggers, float w, float h, int bg);
    void keyBackClicked();
    void onClose(CCObject* sender);
};