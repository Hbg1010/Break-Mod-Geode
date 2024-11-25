#include "../timer/Layers/TimerSettingsLayer.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(TimerPauseLayer, PauseLayer) {
	void customSetup();// {
	// 	PauseLayer::customSetup();

	// 	// auto spr = CircleButtonSprite::createWithSpriteFrameName("particle_197_001.png");
	// 	auto spr = CCSprite::create("TimerSettings.png"_spr);
	// 	spr->setScale(.65f);
	// 	CCMenuItemSpriteExtra* TimersettingsBtn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TimerPauseLayer::onTimerSettings));

	// 	auto settingsMenu = this->getChildByID("right-button-menu");

	// 	// adjusts the right menu to properly allow new buttons to be added
	// 	settingsMenu->setLayout(
	// 		ColumnLayout::create()
	// 			->setGap(5.f)
	// 			->setGrowCrossAxis(true)
	// 			->setAxisReverse(true)
	// 			->setAxisAlignment(AxisAlignment::End)
	// 			,true); 
	// 	// CCMenuItemSpriteExtra* settingsBtn;

	// 	// for (auto* node : CCArrayExt<CCNode*>(settingsMenu->getChildren())) {
	// 	// 	if (node->getID() == "options-button") {
	// 	// 		settingsBtn = static_cast<CCMenuItemSpriteExtra*>(node);
	// 	// 		break;
	// 	// 	}
	// 	// }

	// 	// if (settingsBtn == nullptr) {
	// 	// 	log::error("Options button could not be found!");
	// 	// }

		
	// 	// TimersettingsBtn->setPosition(settingsBtn->getPositionX(), settingsBtn->getPositionY() - settingsBtn->getContentWidth() * 1.5f);
	// 	// TimersettingsBtn->setContentSize(settingsBtn->getScaledContentSize());
	// 	settingsMenu->addChild(TimersettingsBtn);
	// 	TimersettingsBtn->setID("timer-settings-btn"_spr);

	// 	settingsMenu->updateLayout();
	// }

	void onTimerSettings(CCObject* sender);
    //  {
	// 	log::debug("Settings menu was pressed");
	// 	auto layer = TimerSettingsLayer::create(this);
	// 	layer->m_scene = this;
	// 	layer->show();
	// }
};