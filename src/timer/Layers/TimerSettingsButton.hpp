#pragma once
#include "TimerSettingsLayer.hpp"
using namespace geode::prelude;

class TimerSettingsButton final {
private:
    static CCNode* PARENT;

public:
    static CCMenuItemSpriteExtra* create(CCNode* target);
    void onTimerSettings(CCObject* sender);
};