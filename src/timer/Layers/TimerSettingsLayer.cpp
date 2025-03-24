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

// sets up the layer!
bool TimerSettingsLayer::setup(CCNode* const& menuID) {

    TimerSettingsLayer::m_listener = listenForSettingChanges<bool>(menuID->getID() == "PauseLayer" ? "playLayer" : "editorLayer", [this](bool resault) {
        this->updateButtons();
        if (resault) {
            resetTimer(this);
        }
    });

    TimerSettingsLayer::m_menuID = menuID;

    #ifdef extraPrints
        log::debug("{}", TimerSettingsLayer::m_menuID->getID());
    #endif

    if (auto pl = static_cast<TimerPlayLayer*>(TimerPlayLayer::get())) {
        #ifdef extraPrints 
            log::debug("Play Layer");
        #endif
        TimerSettingsLayer::paused = pl->m_fields->paused;
        layerType = PLAYLAYER;

    } else if (auto eui = static_cast<EditorUITimer*>(EditorUITimer::get())){
        #ifdef extraPrints 
            log::debug("Editor Layer");
        #endif
        TimerSettingsLayer::paused = eui->isPaused();
        layerType = EDITOR;

    } else {
        #ifdef extraPrints 
            log::debug("random layer");
        #endif
        layerType = OTHER;
    }

    this->setTitle("Timer Settings!");

    CCMenu* menu = CCMenu::create();
    menu->setPosition(m_mainLayer->getContentWidth()/2, m_mainLayer->getPositionY()/2.f);
    menu->setContentWidth(300.f);
    menu->setLayout(
        RowLayout::create()
        ->setAxisAlignment(AxisAlignment::Even)
    );

    TimerSettingsLayer::menuPointer = menu; // stores pointer

    m_mainLayer->addChild(menu);
    menu->setID("timer-settings-menu"_spr);

    // reset button
    auto resetSpr = CircleButtonSprite::createWithSprite("gold_reset.png"_spr, 1.5f);
    CCMenuItemSpriteExtra* resetButton = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(TimerSettingsLayer::resetTimer)); //menu_selector(TimerSettingsLayer::resetTimer)
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

    // creates the menu where the info button is placed
    auto extraMenu = CCMenu::create();
    extraMenu->setPosition(m_mainLayer->getContentWidth(), m_mainLayer->getContentHeight());
    m_mainLayer->addChild(extraMenu);
    extraMenu->setID("info-menu"_spr);

    // creates the info button
    auto infoSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    CCMenuItemSpriteExtra* infoButton = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(TimerSettingsLayer::createInfoPopup));
    extraMenu->addChild(infoButton);
    infoButton->setID("info-btn"_spr);

    // updates the layout 
    menu->updateLayout();
    updateButtons(); // runs the update fn

    return true;
}

// this callback occurs when the button is clicked
void TimerSettingsLayer::resetTimer(CCObject* sender) {
    // log::debug("{}", TimerSettingsLayer::m_menuID->getID());
    if (layerType == PLAYLAYER) {
        auto layer = static_cast<TimerPlayLayer*>(TimerPlayLayer::get());
        layer->resetTimer();

    } else if (layerType == EDITOR){
        //reseting here is very easy. 
        auto layer = static_cast<EditorUITimer*>(EditorUITimer::get());
		layer->forceReset();
    } else {
        log::error("TimerSettingsLayer not connected to any layer!");
    }
}

// pauses the timer manager on either layer
void TimerSettingsLayer::pauseTime(CCObject* sender) {
    // sets it to the opposite val on click
    TimerSettingsLayer::paused = !TimerSettingsLayer::paused;
    log::debug("this is {}", paused);

    if (layerType == PLAYLAYER) {
        static_cast<TimerPlayLayer*>(TimerPlayLayer::get())->pauseTimer(TimerSettingsLayer::paused);
    } else if (layerType == EDITOR) {
        static_cast<EditorUITimer*>(EditorUITimer::get())->pauseTimer(TimerSettingsLayer::paused);
    } else {
        log::error("TimerSettingsLayer not connected to any layer!");
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

// shows info when the info button is pressed
void TimerSettingsLayer::createInfoPopup(CCObject* sender) {
    auto x = FLAlertLayer::create("Info", 
    "<cy>Reset Button:</c> Resets the timer\n<cy>Pause Button:</c> Pauses the timer\n<cy>Settings Button:</c> Opens mod settings", "Ok");
    x->setTouchPriority(this->getTouchPriority()-1);
    x->show();
    x->setID("timer-info-popup"_spr);
}

// dtor to drop this global listener.
TimerSettingsLayer::~TimerSettingsLayer() {
    m_listener->disable();
    CC_SAFE_DELETE(m_listener);
}