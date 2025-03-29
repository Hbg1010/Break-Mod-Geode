#include "TimerPauseLayer.hpp"

void TimerPauseLayer::customSetup() {
	PauseLayer::customSetup();
	
	// early return if the timer button is disabled
    if (!Mod::get()->getSettingValue<bool>("useQuickSettings")) return; 

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