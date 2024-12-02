#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "../../hooks/EditorUITimer.hpp"
#include "../../hooks/TimerPlayLayer.hpp"

using namespace geode::prelude;

class TimerSettingsLayer : public Popup<CCNode* const&> {
public:
    static TimerSettingsLayer* create(CCNode* const& menuID);
    static void disableButton(CCNode* node, bool enable);

    // static TimerSettingsLayer* get();

protected:
    CCNode* m_menuID;
    bool paused;
    CCMenu* menuPointer;
    // bool isDisabled;

    bool setup(CCNode* const& menuID);
    void resetTimer(CCObject* sender);
    void changeSettings(CCObject* sender);
    void pauseTime(CCObject* sender);

    void updateButtons();
    ~TimerSettingsLayer();
};