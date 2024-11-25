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

// this executes the event when it occurs
$execute{

	// handles popups on both editor and pause layer
	new EventListener<EventFilter<TimerEvent>>(+[](TimerEvent* ev) {
		if (ev->isActive()) {

			std::string timerAlert = fmt::format("Time to take a {} second break!", Mod::get()->getSettingValue<int64_t>("breakTime"));

			// creates popup before the timer
			auto x = geode::createQuickPopup("Timer", timerAlert, "Start", "Skip", [ev](auto, bool btn2) {
				
					// yeah I hate that not, but the constructor doesnt allow me to flip the things otherwise!
					if (!btn2) {
						// adds a new node to the layer parent
						CCNode* x = CCNode::create(); 
						TimerLayer* timePopUp = TimerLayer::create(fmt::format("{}", Mod::get()->getSettingValue<int64_t>("breakTime")));

						// returns the grandfather of a node, so The popup is placed at the top of the tree
						CCNode* parent = ev->getCurrentLayer(); 

						while(parent->getParent() != nullptr) {
							parent = parent->getParent();
						}
						timePopUp->m_noElasticity = CCDirector::get()->getFastMenu();
						parent->addChild(timePopUp, 20);
						timePopUp->setID("Timer-pop-up"_spr);
				} 
		}, false);

		// displays x
		x->m_noElasticity = CCDirector::get()->getFastMenu();
		x->show();

		// if in editor, this will set the touch priority above the EditorPauseLayer
		// very open to knowing a better way to do this! ! ! Unlike the editorUI though, theres no get function :(
		auto editorPauseLayer = ev->getCurrentLayer()->getParent()->getChildByID("EditorPauseLayer");
		if (editorPauseLayer != nullptr) x->setTouchPriority(static_cast<EditorPauseLayer*>(editorPauseLayer)->getTouchPriority() - 1);

		x->setID("TimerAlert"_spr);

			return ListenerResult::Stop;

		}
		return ListenerResult::Propagate;
	});
};