#pragma once
#include "TimerSettingsLayer.hpp"
using namespace geode::prelude;

class TimerSettingsButton final {
public:
    static CCMenuItemSpriteExtra* create(CCNode* target);
    void onTimerSettings(CCObject* sender);
};