/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include "timer/TimerEvent.hpp"
#include "timer/TimerLayer.hpp"

/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;

#ifdef SHORTEN_TIMER_TIME 
	#define TMULT 1
#else
	#define TMULT 60
#endif

$on_mod(Loaded) {
	Mod::get()->setSavedValue<float>("savedTime", -1); // idk how to delete this setting / make it useless on init
    if (Mod::get()->getSettingValue<bool>("resetPause")) Mod::get()->setSavedValue<bool>("timerPaused", false);

    #ifdef extraPrints
        log::debug("settings reset!");
    #endif
};

$execute{
	// handles popups on both editor and pause layer
	new EventListener<EventFilter<TimerEvent>>(+[](TimerEvent* ev) {
		if (ev->isActive()) {
			// resets timer sav val if needed on event
			if (Mod::get()->getSettingValue<bool>("useSaving")) 
				Mod::get()->setSavedValue<float>("savedTime", Mod::get()->getSettingValue<int>("interval") * TMULT);
			
			CCNode* parent = ev->getCurrentLayer(); 

			while(parent->getParent() != nullptr) {
				parent = parent->getParent();
			}

			// should stop copies being created
			if (parent->getChildByID("TimerAlert"_spr) != nullptr && parent->getChildByID("Timer-pop-up"_spr) != nullptr) {
				log::error("timer layer already exists");
				return ListenerResult::Stop;
			}

			// creates popup before the timer
			auto x = geode::createQuickPopup("Timer",
				fmt::format("Time to take a {} second break!", 
				Mod::get()->getSettingValue<int64_t>("breakTime")), 
				"Start", "Skip", 
				[ev](auto, bool btn2) {
					// yeah I hate that !btn2, but the constructor doesnt allow me to flip the things otherwise!
					if (!btn2) {
						TimerLayer* timePopUp = TimerLayer::create(
							fmt::format("{}", Mod::get()->getSettingValue<int64_t>("breakTime")));
						timePopUp->m_noElasticity = CCDirector::get()->getFastMenu();
						timePopUp->show();
						timePopUp->setID("Timer-pop-up"_spr);
					} 
				}, false);		

			// displays x
			x->m_noElasticity = CCDirector::get()->getFastMenu(); // stops intro animation when fast reset is used
			x->show();
			x->setID("TimerAlert"_spr);

			/*
			if in editor, this will set the touch priority above the EditorPauseLayer
			very open to knowing a better way to do this! ! ! Unlike the editorUI though, theres no get function :(
			also set to -2 so that it has a higher priority than the timer pause, solving conflicts!
			*/
			auto editorPauseLayer = ev->getCurrentLayer()->getParent()->getChildByID("EditorPauseLayer");
			if (editorPauseLayer != nullptr) x->setTouchPriority(static_cast<EditorPauseLayer*>(editorPauseLayer)->getTouchPriority() - 2);

			// returns that the event ended!
			return ListenerResult::Stop;
		}

			return ListenerResult::Propagate;
	});
};