#include "TimerSettingsLayer.hpp"

TimerSettingsLayer* TimerSettingsLayer::create(std::string const& menuID) {
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

bool TimerSettingsLayer::setup(std::string const& menuID) {
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    log::debug("{}", menuID);

    this->setTitle("Timer Settings!");

    CCMenu* menu = CCMenu::create();

    menu->setContentWidth(300.f);
    menu->setLayout(
        RowLayout::create()
        ->setAxisAlignment(AxisAlignment::Even)
    );

    this->addChild(menu);
    menu->setID("timer-settings-menu"_spr);
    return true;
}