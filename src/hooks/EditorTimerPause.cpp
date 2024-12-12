#include "EditorTimerPause.hpp"


// void EditorTimerPause::onTimerSettings(CCObject* sender) {
//     log::debug("Settings menu was pressed");
//     auto layer = TimerSettingsLayer::create(this);
//     layer->m_scene = this;
//     layer->setTouchPriority(this->getTouchPriority() -1);
//     layer->show();
// }

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
            // ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Center)
            ,true); 


    settingsMenu->addChild(timersettingsBtn);
    timersettingsBtn->setID("timer-settings-btn"_spr);

    settingsMenu->updateLayout();
    return true;
}

void EditorTimerPause::onResume(CCObject* sender) {
    EditorPauseLayer::onResume(sender);
    auto x = static_cast<EditorUITimer*>(EditorUITimer::get());

    if (Mod::get()->getSettingValue<bool>("editorLayer") && !x->m_fields->paused) {
        x->resetTimer();
    }
    

}