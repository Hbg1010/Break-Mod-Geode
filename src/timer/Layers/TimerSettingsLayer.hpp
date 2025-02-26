#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Popup.hpp>
#include "../../hooks/EditorUITimer.hpp"
#include "../../hooks/TimerPlayLayer.hpp"
#include "../../utils/devMode.hpp"

using namespace geode::prelude;

class TimerSettingsLayer : public Popup<CCNode* const&> {

// friend class modSettingsEvent;

public:
    static TimerSettingsLayer* create(CCNode* const& menuID);
    static void disableButton(CCNode* node, bool enable);
    static TimerSettingsLayer* get();

protected:

    CCNode* m_menuID; // ptr to buttons attatched to this layer
    enum LayerEnum {
        OTHER, PLAYLAYER, EDITOR
    };

    LayerEnum layerType;
    EventListener<SettingChangedFilter>* m_listener;
    bool paused;
    CCMenu* menuPointer;
  
    bool setup(CCNode* const& menuID);
    void resetTimer(CCObject* sender);
    void changeSettings(CCObject* sender);
    void pauseTime(CCObject* sender);
    void updateButtons();
    void createInfoPopup(CCObject* sender);
    ~TimerSettingsLayer();
};