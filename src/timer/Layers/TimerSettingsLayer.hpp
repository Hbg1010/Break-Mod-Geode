#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "../../hooks/EditorUITimer.hpp"
#include "../../hooks/TimerPlayLayer.hpp"

using namespace geode::prelude;

class TimerSettingsLayer : public Popup<CCNode* const&> {
public:
    static TimerSettingsLayer* create(CCNode* const& menuID);

protected:
    CCNode* m_menuID;
    bool paused;
    bool setup(CCNode* const& menuID);
    void resetTimer(CCObject* sender);
    void changeSettings(CCObject* sender);
    void pauseTime(CCObject* sender);

    void updateButtons();
};