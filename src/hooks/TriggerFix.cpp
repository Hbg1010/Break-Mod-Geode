#include "TriggerFix.hpp"

bool TriggerFix::init(EffectGameObject* trigger, CCArray* triggers, float w, float h, int bg) {
    if (!SetupTriggerPopup::init(trigger, triggers, w, h, bg)) return false;

    if (auto EUI = static_cast<EditorUITimer*>(EditorUITimer::get())) {
        if (!Mod::get()->getSettingValue<bool>("interuptTriggers")) EUI->setDelayedFlag(true);
    }

    return true;
}

void TriggerFix::keyBackClicked() {
    SetupTriggerPopup::keyBackClicked();
    if (auto EUI = static_cast<EditorUITimer*>(EditorUITimer::get())) {
        EUI->setDelayedFlag(false);
        EUI->checkEndPlaytest();
    }
}
void TriggerFix::onClose(CCObject* sender) {
    SetupTriggerPopup::onClose(sender);

    if (auto EUI = static_cast<EditorUITimer*>(EditorUITimer::get())) {
        EUI->setDelayedFlag(false);
        EUI->checkEndPlaytest();
    }
}