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

    menuPointer = CCMenu::create();
    menuPointer->setPosition(m_mainLayer->getContentWidth()/2, m_mainLayer->getContentHeight()/2.f);
    menuPointer->setContentWidth(300.f);
    menuPointer->setLayout(
        RowLayout::create()
        ->setAxisAlignment(AxisAlignment::Even)
    );

    m_mainLayer->addChild(menuPointer);
    menuPointer->setID("timer-settings-menu"_spr);

    // reset button
    auto resetSpr = CircleButtonSprite::createWithSpriteFrameName("gold_reset.png"_spr, 1.5f);
    resetSpr->setScale(1.25f);
    CCMenuItemSpriteExtra* resetButton = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(TimerSettingsLayer::resetTimer)); //menu_selector(TimerSettingsLayer::resetTimer)
    menuPointer->addChild(resetButton);
    resetButton->setID("reset-button"_spr);
    
    // add button
    auto pauseSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");//"GJ_plusBtn_001.png"
    CCMenuItemSpriteExtra* pauseButton = CCMenuItemSpriteExtra::create(pauseSpr, this, menu_selector(TimerSettingsLayer::pauseTime));
    menuPointer->addChild(pauseButton);
    pauseButton->setID("pause-button"_spr);

    // settings button
    auto settingsSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");//"GJ_optionsBtn_001.png"
    settingsSpr->setScale(1.25f);
    CCMenuItemSpriteExtra* settingsButton = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(TimerSettingsLayer::changeSettings));
    menuPointer->addChild(settingsButton);
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
    menuPointer->updateLayout();
    updateButtons(); // runs the update fn

    // This constructs a listener for editor stuff
    TimerSettingsLayer::m_mode_listener = listenForSettingChanges<bool>(menuID->getID() == "PauseLayer" ? "playLayer" : "editorLayer", [this](bool resault) {
        this->updateButtons();

        // handles the timers 
        if (resault) {
            resetTimer(this);
        } else {
            if (Mod::get()->getSettingValue<bool>("useSaving")) {
                double toSave;
                if (layerType == PLAYLAYER) {
                    auto layer = static_cast<TimerPlayLayer*>(TimerPlayLayer::get());
                    toSave = layer->remainingSeconds();
            
                } else if (layerType == EDITOR) {
                    auto layer = static_cast<EditorUITimer*>(EditorUITimer::get());
                    toSave = layer->getRemainder();
                } else {
                    return ListenerResult::Propagate;
                }

                if (toSave > 0) {
                    Mod::get()->setSavedValue<float>("savedTime", toSave);
                } else Mod::get()->setSavedValue<float>("savedTime", 5); // this is just so the editor (hopefully) wont freak out :)
            }
        }
        return ListenerResult::Propagate;
    });

    // listener for resetting the timer when interval is changed in level
    m_interval_listener = listenForSettingChanges<int64_t>("interval", [this](int changed) {
        if (Mod::get()->getSettingValue<bool>("resetOnInterval")) resetTimer(this);
        return ListenerResult::Propagate;
    });

    return true;
}

// this callback occurs when the button is clicked
void TimerSettingsLayer::resetTimer(CCObject* sender) {
    if (layerType == PLAYLAYER) {
        auto layer = static_cast<TimerPlayLayer*>(TimerPlayLayer::get());
        layer->resetTimer();

    } else if (layerType == EDITOR){
        auto layer = static_cast<EditorUITimer*>(EditorUITimer::get());
		layer->forceReset();
    } else {
        log::error("TimerSettingsLayer not connected to any layer!");
    }
}

// pauses the timer manager on either layer
void TimerSettingsLayer::pauseTime(CCObject* sender) {
    TimerSettingsLayer::paused = !TimerSettingsLayer::paused; // sets it to the opposite val on click

    // saves when setting is enabled
    if (Mod::get()->getSettingValue<bool>("pauseAcrossLevels")) Mod::get()->setSavedValue<bool>("timerPaused", TimerSettingsLayer::paused);

    #ifdef extraPrints
        log::debug("this is {}", paused);
    #endif

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

    if (TimerSettingsLayer::paused) {
        sprite->setScale(.8125f);
    } else {
        sprite->setScale(1.59375f);
    }

    btnPointer->setSprite(sprite);

    // checks if the pause and play should be enabled
    bool currentLayerEnabled = (m_menuID->getID() == "PauseLayer" && Mod::get()->getSettingValue<bool>("playLayer")) || (m_menuID->getID() == "EditorPauseLayer" && Mod::get()->getSettingValue<bool>("editorLayer"));

    for (CCNode* node : CCArrayExt<CCNode*>(menuPointer->getChildren())) {
        if (node->getID() != "settings-button"_spr) {
            TimerSettingsLayer::disableButton(node, currentLayerEnabled);
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

// dtor to drop the global listeners.
TimerSettingsLayer::~TimerSettingsLayer() {
    m_mode_listener->disable();
    CC_SAFE_DELETE(m_mode_listener);
    m_interval_listener->disable();
    CC_SAFE_DELETE(m_interval_listener);
}