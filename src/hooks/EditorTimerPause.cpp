#include "EditorTimerPause.hpp"

bool EditorTimerPause::init(LevelEditorLayer* lvl) {

    if (!EditorPauseLayer::init(lvl)) return false;

    // auto spr = CircleButtonSprite::createWithSpriteFrameName("particle_197_001.png");
    auto spr = CCSprite::create("TimerSettings.png"_spr);
    spr->setScale(.65f);
    CCMenuItemSpriteExtra* timersettingsBtn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(EditorTimerPause::onTimerSettings));

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

    x->resetTimer();

}

void EditorTimerPause::onTimerSettings(CCObject* sender) {
    log::debug("Settings menu was pressed");
    auto layer = TimerSettingsLayer::create(this);
    layer->m_scene = this;
    layer->setTouchPriority(this->getTouchPriority() -1);
    layer->show();
}