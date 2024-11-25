#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class TimerSettingsLayer : public Popup<std::string const&> {

    public:
        static TimerSettingsLayer* create(std::string const& menuID);
        std::string m_menuID;

    protected:
        bool setup(std::string const& menuID);
        void resetTimer(CCObject* sender);
        void changeSettings(CCObject* sender);
        void addTime(CCObject* sender);
};