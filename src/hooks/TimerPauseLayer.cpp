#include "TimerPauseLayer.hpp"

void TimerPauseLayer::customSetup() {
	PauseLayer::customSetup();

	// auto spr = CircleButtonSprite::createWithSpriteFrameName("particle_197_001.png");
	// auto spr = CCSprite::create("TimerSettings.png"_spr);
	// spr->setScale(.65f);
	// CCMenuItemSpriteExtra* TimersettingsBtn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TimerPauseLayer::onTimerSettings));

    if (!Mod::get()->getSettingValue<bool>("useQuickSettings")) return; // early return if the timer button is disabled

	CCMenuItemSpriteExtra* TimersettingsBtn = TimerSettingsButton::create(this);

	auto settingsMenu = this->getChildByID("right-button-menu");

	// adjusts the right menu to properly allow new buttons to be added
	settingsMenu->setLayout(
		ColumnLayout::create()
			->setGap(5.f)
			->setGrowCrossAxis(true)
			->setAxisReverse(true)
			->setAxisAlignment(AxisAlignment::End)
			,true); 

	settingsMenu->addChild(TimersettingsBtn);
	TimersettingsBtn->setID("timer-settings-btn"_spr);

	settingsMenu->updateLayout();
}