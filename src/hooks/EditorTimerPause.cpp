#include "EditorTimerPause.hpp"

void EditorTimerPause::onResume(CCObject* sender) {
    EditorPauseLayer::onResume(sender);
    auto layer = static_cast<EditorUITimer*>(EditorUITimer::get());

    if (Mod::get()->getSettingValue<bool>("editorLayer") && !layer->isPaused()) {
        layer->resetTimer();
    }
}

/* hooks
========== */

bool EditorTimerPause::init(LevelEditorLayer* lvl) {

    if (!EditorPauseLayer::init(lvl)) return false;
    
    if (!Mod::get()->getSettingValue<bool>("useQuickSettings")) return true; // early return if the timer button is disabled

    CCMenuItemSpriteExtra* timersettingsBtn = TimerSettingsButton::create(this);

    auto settingsMenu = this->getChildByID("settings-menu");

    settingsMenu->setContentHeight(this->getContentHeight()*2);
    settingsMenu->setPositionY(settingsMenu->getPositionY()+5.f);
        settingsMenu->setLayout(
        ColumnLayout::create()
            ->setGap(5.f)
            ->setGrowCrossAxis(true)
            ->setAxisAlignment(AxisAlignment::Center)
            ,true); 


    settingsMenu->addChild(timersettingsBtn);
    timersettingsBtn->setID("timer-settings-btn"_spr);

    settingsMenu->updateLayout();
    return true;
}