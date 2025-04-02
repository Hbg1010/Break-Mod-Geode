#include "EditorTimerPause.hpp"

/* hooks
========== */

bool EditorTimerPause::init(LevelEditorLayer* lvl) {
    if (!EditorPauseLayer::init(lvl)) return false;

    // early return if the timer button is disabled
    if (Mod::get()->getSettingValue<bool>("useQuickSettings")) {
        CCMenuItemSpriteExtra* timersettingsBtn = TimerSettingsButton::create(this);

        auto settingsMenu = this->getChildByID("settings-menu");

        settingsMenu->setContentHeight(this->getContentHeight()*2);
        settingsMenu->setPositionY(settingsMenu->getPositionY()+5.f);
            settingsMenu->setLayout(
            ColumnLayout::create()
                ->setGap(5.f)
                ->setGrowCrossAxis(true)
                ->setAxisAlignment(AxisAlignment::Center),
            true); 

        settingsMenu->addChild(timersettingsBtn);
        timersettingsBtn->setID("timer-settings-btn"_spr);

        settingsMenu->updateLayout();
    }
    return true;
}

void EditorTimerPause::onResume(CCObject* sender) {
    EditorPauseLayer::onResume(sender);
    auto leui = static_cast<EditorUITimer*>(EditorUITimer::get());

    if (leui != nullptr && Mod::get()->getSettingValue<bool>("editorLayer") && !leui->isPaused()) {
        int remainder = leui->getRemainder();
        #ifdef extraPrints
        log::debug("{}", remainder);
        #endif
        leui->resetTimer(remainder > 0 
            ? remainder 
            : Mod::get()->getSettingValue<int64_t>("interval") * 60);
    }
}

// when exiting the editor, this should save the timer between sessions
void EditorTimerPause::onExitEditor(cocos2d::CCObject* sender) {
    if (auto leui = static_cast<EditorUITimer*>(EditorUITimer::get())) {
        if (Mod::get()->getSettingValue<bool>("useSaving") && Mod::get()->getSettingValue<bool>("editorLayer")) {
            float time = (float) leui->m_fields->remainingTime;
            if (time > 0) Mod::get()->setSavedValue<float>("savedTime", time);

            #ifdef extraPrints
            float temp = Mod::get()->getSavedValue<float>("savedTime", time);
            log::debug("{}", temp);
            #endif
        }

        leui->cancelTimer(); // this may help memory?
    }

    EditorPauseLayer::onExitEditor(sender);
}