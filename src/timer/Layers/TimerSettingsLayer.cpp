#include "TimerSettingsLayer.hpp"

TimerSettingsLayer* TimerSettingsLayer::create(CCNode* const& menuID) {
    auto temp = new TimerSettingsLayer();

    // trys to make node
    if (temp->initAnchored(300, 150, menuID, "GJ_square02.png")) {
        temp->autorelease();
        return temp;

    } else {
        CC_SAFE_DELETE(temp);

        return nullptr;
    }
}

bool TimerSettingsLayer::setup(CCNode* const& menuID) {
    TimerSettingsLayer::m_menuID = menuID;
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    log::debug("{}", menuID);

    this->setTitle("Timer Settings!");

    CCMenu* menu = CCMenu::create();

    menu->setContentWidth(300.f);
    menu->setLayout(
        RowLayout::create()
        ->setAxisAlignment(AxisAlignment::Even)
    );

    this->addChild(menu);
    menu->setID("timer-settings-menu"_spr);

    // reset button
    auto resetSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");//"GJ_updateBtn_001.png" 
    CCMenuItemSpriteExtra* resetButton = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(TimerSettingsLayer::resetTimer)); //menu_selector(TimerSettingsLayer::resetTimer)
    menu->addChild(resetButton);
    resetButton->setID("reset-button"_spr);

    // add button
    auto addSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");//"GJ_plusBtn_001.png"
    CCMenuItemSpriteExtra* addButton = CCMenuItemSpriteExtra::create(addSpr, this, nullptr);
    menu->addChild(addButton);
    addButton->setID("add-button"_spr);

    //TODO: Find propper nodeIDs
    // disables buttons if the current layer is disabled.
    if ((!Mod::get()->getSettingValue<bool>("playLayer") && menuID->getID() == "PauseLayer") || (!Mod::get()->getSettingValue<bool>("editorLayer") && menuID->getID() == "EditorPauseLayer")) {
        resetButton->setEnabled(false);
        addButton->setEnabled(false);
    }

    // settings button
    auto settingsSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");//"GJ_optionsBtn_001.png"
    CCMenuItemSpriteExtra* settingsButton = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(TimerSettingsLayer::changeSettings));
    menu->addChild(settingsButton);
    settingsButton->setID("settings-button"_spr);

    // updates the layout 
    menu->updateLayout();
    return true;
}

// this callback occurs when the button is clicked
void TimerSettingsLayer::resetTimer(CCObject* sender) {
    log::debug("{}", TimerSettingsLayer::m_menuID->getID());
    if (TimerSettingsLayer::m_menuID->getID() == "PauseLayer") {
        auto layer = static_cast<TimerPlayLayer*>(TimerPlayLayer::get());
        layer->m_fields->resetTimer();

    } else {
        //reseting here is very easy. 
        auto layer = EditorUITimer::get();
        auto x = static_cast<EditorUITimer*>(EditorUITimer::get());
		x->forceReset();
    }
}

void TimerSettingsLayer::pauseTime(CCObject* sender) {
    log::debug("paused");

    // sets it to the opposite val on click
    TimerSettingsLayer::paused = !TimerSettingsLayer::paused;

    if (TimerSettingsLayer::m_menuID->getID() == "PauseLayer") {
        auto layer = static_cast<TimerPlayLayer*>(TimerPlayLayer::get());

        layer->pauseTimer(TimerSettingsLayer::paused);
    } else {
        log::debug("todo");
    }

}

// opens geode settings page
void TimerSettingsLayer::changeSettings(CCObject* sender) {
    geode::openSettingsPopup(Mod::get());

    /*
    TODO: 
    */
}

$execute {
    new EventListener<EventFilter<ModLogoUIEvent>>(+[](ModLogoUIEvent* event) {
        if (event->getModID() == "hbg1010.20_20_20") {
            log::debug("hbg");
        }
        // You should always propagate Geode UI events
        return ListenerResult::Propagate;
    });
}
