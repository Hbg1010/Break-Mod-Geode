#include "TimerSettingsLayer.hpp"
#include <memory>

// EventListener<SettingChangedFilterV3> m_listener = {this, upd};

// geode::EventListener<Popup::CloseEventFilter> m_listener = {
//     this, &TimerSettingsLayer::updateButtons(), CloseEventFilter(this, )
// }

TimerSettingsLayer* TimerSettingsLayer::create(CCNode* const& menuID) {
    auto temp = new TimerSettingsLayer();

    // trys to make node
    if (temp->initAnchored(300, 150, menuID, "GJ_square02.png")) {
        temp->autorelease();
        return temp;

    } else {
        // CC_SAFE_DELETE(temp);

        return nullptr;
    }
}

// sets up the layer!
bool TimerSettingsLayer::setup(CCNode* const& menuID) {


    // TimerSettingsLayer::m_listener = listenForSettingChanges<bool>(menuID->getID() == "PauseLayer" ? "playLayer" : "editorLayer", &TimerSettingsLayer::updateButtons);
    TimerSettingsLayer::m_listener = listenForSettingChanges<bool>(menuID->getID() == "PauseLayer" ? "playLayer" : "editorLayer", [this](bool resault) {
        this->updateButtons();
        if (resault) {
            resetTimer(this);
        }
    });

    TimerSettingsLayer::m_menuID = menuID;
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    // log::debug("{}", menuID);

    if (TimerSettingsLayer::m_menuID->getID() == "PauseLayer") {
        TimerSettingsLayer::paused = static_cast<TimerPlayLayer*>(TimerPlayLayer::get())->m_fields->paused;
    } else {
        TimerSettingsLayer::paused = static_cast<EditorUITimer*>(EditorUITimer::get())->m_fields->paused;
    }

    this->setTitle("Timer Settings!");


        // reset button
    auto resetSpr = cocos2d::CCSprite::create("TM_replayBtn.png"_spr);//"GJ_updateBtn_001.png" 
    resetSpr->setScale(.75f);
    CCMenuItemSpriteExtra* resetButton = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(TimerSettingsLayer::resetTimer)); //menu_selector(TimerSettingsLayer::resetTimer)

    CCMenu* menu = CCMenu::create();
    menu->setPosition({(m_mainLayer->getPositionX() + resetButton->getContentWidth() * 1.25f)/2.f, m_mainLayer->getPositionY()/2.f});
    menu->setContentWidth(300);
    menu->setLayout(
        RowLayout::create()
        ->setAxisAlignment(AxisAlignment::Even)
    );

    TimerSettingsLayer::menuPointer = menu; // stores pointer

    m_mainLayer->addChild(menu);
    // this->addChildAtPosition(menu,Anchor::Center);
    menu->setID("timer-settings-menu"_spr);


    menu->addChild(resetButton);
    resetButton->setID("reset-button"_spr);
    

    // add button
    auto pauseSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");//"GJ_plusBtn_001.png"
    CCMenuItemSpriteExtra* pauseButton = CCMenuItemSpriteExtra::create(pauseSpr, this, menu_selector(TimerSettingsLayer::pauseTime));
    menu->addChild(pauseButton);
    pauseButton->setID("pause-button"_spr);

    // settings button
    auto settingsSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");//"GJ_optionsBtn_001.png"
    CCMenuItemSpriteExtra* settingsButton = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(TimerSettingsLayer::changeSettings));
    menu->addChild(settingsButton);
    settingsButton->setID("settings-button"_spr);

    // updates the layout 
    menu->updateLayout();

    updateButtons();

    // m_listener.setFilter(SettingChangedFilterV3(Mod::get(), "playLayer"));
    // m_listener.bind([&](SettingChangedEventV3* ev) {
    //     log::debug("IM GONNA KMS");
    // });

    // m_listener = EventListener<SettingChangedFilterV3>(this, &onSettingsChanged, SettingChangedFilterV3(Mod::get(), "playLayer"));
    // TimerSettingsLayer::m_listener.setFilter(SettingChangedFilterV3(Mod::get(), "playLayer"));
    // TimerSettingsLayer::m_listener.bind(&[](SettingChangedEventV3* ev) {

    // });

    // m_listener.bind(this, &TimerSettingsLayer::onSettingsChanged);
    //  = {
    //     this, updateButtons(), SettingChangedFilterV3(Mod::get(), "playLayer")
    // };

    return true;
}

// this callback occurs when the button is clicked
void TimerSettingsLayer::resetTimer(CCObject* sender) {
    // log::debug("{}", TimerSettingsLayer::m_menuID->getID());
    if (TimerSettingsLayer::m_menuID->getID() == "PauseLayer") {
        auto layer = static_cast<TimerPlayLayer*>(TimerPlayLayer::get());
        layer->m_fields->resetTimer();

    } else {
        //reseting here is very easy. 
        auto layer = EditorUITimer::get();
        auto x = static_cast<EditorUITimer*>(EditorUITimer::get());
        if (paused) {
		    x->forceReset();
        } else {
            x->m_fields->remainingTime = Mod::get()->getSettingValue<int64_t>("interval");
        }
    }
}

// pauses the timer manager on either layer
void TimerSettingsLayer::pauseTime(CCObject* sender) {
    // sets it to the opposite val on click
    TimerSettingsLayer::paused = !TimerSettingsLayer::paused;
    log::debug("this is {}", paused);

    if (TimerSettingsLayer::m_menuID->getID() == "PauseLayer") {
        static_cast<TimerPlayLayer*>(TimerPlayLayer::get())->pauseTimer(TimerSettingsLayer::paused);
    } else {
        static_cast<EditorUITimer*>(EditorUITimer::get())->pauseTimer(TimerSettingsLayer::paused);
    }

    updateButtons();

}

// opens geode settings page
void TimerSettingsLayer::changeSettings(CCObject* sender) {
   auto settingsPopup = geode::openSettingsPopup(Mod::get(), true);
    
}

// this will update the visuals shown by the ui
void TimerSettingsLayer::updateButtons() {
    CCMenuItemSpriteExtra* btnPointer = static_cast<CCMenuItemSpriteExtra*>(TimerSettingsLayer::menuPointer->getChildByID("pause-button"_spr));
    if (btnPointer == nullptr) {
        log::error("Button is nullptr");
    }

    auto sprite = cocos2d::CCSprite::createWithSpriteFrameName(TimerSettingsLayer::paused ? "GJ_playBtn2_001.png" : "GJ_pauseEditorBtn_001.png");

    // TODO: ADJUST SIZES SLIGHTLY
    if (TimerSettingsLayer::paused) {
        sprite->setScale(.65f);
    } else {
        sprite->setScale(1.275f);
    }

    btnPointer->setSprite(sprite);

    // checks if the pause and play should be enabled
    bool enabled = (Mod::get()->getSettingValue<bool>("playLayer") && m_menuID->getID() == "PauseLayer") || (Mod::get()->getSettingValue<bool>("editorLayer") && m_menuID->getID() == "EditorPauseLayer");

    for (CCNode* node : CCArrayExt<CCNode*>(menuPointer->getChildren())) {
        if (node->getID() != "settings-button"_spr) {
            TimerSettingsLayer::disableButton(node, enabled);
        }
    }
}

// static function to disable a button on call
void TimerSettingsLayer::disableButton(CCNode* node, bool enable) {
    //TODO Find a better spot to put this
    const ccColor3B greyScale = {.r = 90, .g = 90, .b = 90};
    const ccColor3B color = {.r = 255, .g = 255, .b = 255};

    if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
        btn->setEnabled(enable);
        auto spr = typeinfo_cast<CCRGBAProtocol*>(btn->getNormalImage());
            spr->setCascadeColorEnabled(true);
            spr->setCascadeOpacityEnabled(true);
            spr->setColor(enable ? color : greyScale);
            spr->setOpacity(enable ? 255 : 200);
    }
}

// dtor to drop this global listener.
TimerSettingsLayer::~TimerSettingsLayer() {
    m_listener->disable();
    CC_SAFE_DELETE(m_listener);
}