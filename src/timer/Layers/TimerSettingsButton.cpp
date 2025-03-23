#include "TimerSettingsButton.hpp"

CCNode* TimerSettingsButton::PARENT = nullptr;

CCMenuItemSpriteExtra* TimerSettingsButton::create(CCNode* target) {
    PARENT = target;
    auto spr = CCSprite::create("TimerSettings.png"_spr);
	spr->setScale(.65f);
	CCMenuItemSpriteExtra* TimersettingsBtn = CCMenuItemSpriteExtra::create(spr, target, menu_selector(TimerSettingsButton::onTimerSettings));
    
    return TimersettingsBtn;
}

// this is the buttons callback
void TimerSettingsButton::onTimerSettings(CCObject* sender) {
    log::debug("Settings menu was pressed");
    TimerSettingsLayer* layer = TimerSettingsLayer::create(PARENT);
    layer->m_scene = PARENT;

    if (PARENT->getID() == "EditorPauseLayer") {
        log::debug("a");
        layer->setTouchPriority(static_cast<CCLayer*>(PARENT)->getTouchPriority() - 1);
    }

    // stops opening effect if fast menu is used
    layer->m_noElasticity = CCDirector::get()->getFastMenu();

    layer->show();
}