#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class TimerSettingsLayer : public Popup<std::string const&> {

    public:
        static TimerSettingsLayer* create(std::string const& menuID);

    protected:
        bool setup(std::string const& menuID);
        void resetTimer();
        void changeSettings();
        void addTime();
};