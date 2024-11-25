#include "TimerPauseLayer.hpp"


    void TimerPauseLayer::onTimerSettings(CCObject* sender) {
        log::debug("Settings menu was pressed");
		auto layer = TimerSettingsLayer::create(this);
		layer->m_scene = this;
		layer->show();
    }

/* hooks
========== */

void TimerPauseLayer::customSetup() {
	PauseLayer::customSetup();

	// auto spr = CircleButtonSprite::createWithSpriteFrameName("particle_197_001.png");
	auto spr = CCSprite::create("TimerSettings.png"_spr);
	spr->setScale(.65f);
	CCMenuItemSpriteExtra* TimersettingsBtn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TimerPauseLayer::onTimerSettings));

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