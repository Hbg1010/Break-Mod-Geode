#pragma once

#include <Geode/Geode.hpp>
#include "../../hooks/EditorUITimer.hpp"
#include "../../hooks/TimerPlayLayer.hpp"


using namespace geode::prelude;

class TimerSettingsLayer : public Popup<CCNode* const&> {
public:
    static TimerSettingsLayer* create(CCNode* const& menuID);

protected:
    CCNode* m_menuID;
    bool setup(CCNode* const& menuID);
    void resetTimer(CCObject* sender);
    void changeSettings(CCObject* sender);
    void addTime(CCObject* sender);
};