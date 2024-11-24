/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <chrono>
#include <ctime>
#include "timer/TimerEvent.cpp"
#include "timer/TimerLayer.cpp"
#include "timer/EditorTimer.cpp"

/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;

// Controls pausing in game.
#include <Geode/modify/PlayLayer.hpp> 
class $modify(PlayLayer) {

	struct Fields {
		// std::chrono::time_point<std::chrono::system_clock> starttime;
		std::chrono::time_point<std::chrono::system_clock> endtime;
		bool useTimer;
		// resets the timer on call;
		void resetTimer() {
			endtime = std::chrono::system_clock::now() + std::chrono::minutes{Mod::get()->getSettingValue<int64_t>("interval")}; // TODO: Replace with setting
		}
	};

	bool init(GJGameLevel* p0, bool p1, bool p2){
		if (!PlayLayer::init(p0,p1,p2)) return false;
		// NodeIDs::provideFor(this);
		m_fields->useTimer = Mod::get()->getSettingValue<bool>("playLayer");

		if (m_fields->useTimer) {
			auto tp1 = std::chrono::system_clock::now() ;

		// m_fields->starttime = std::chrono::system_clock::now();
		m_fields->resetTimer();
		}
		
	
		// std::time_t time = std::chrono::system_clock::to_time_t(m_fields->endtime);
		// log::debug("TIme is {}", std::ctime(&time));
		return true;
	}
		// "dependencies": [
		// 	{
		// 		"id": "geode.node-ids",
		// 		"version": "v1.17.0-beta.1",
		// 		"importance": "required"
		// 	}
		// ]

	void resetLevel() {
		PlayLayer::resetLevel();

		if (!m_fields->useTimer) return;

		// condition. checks if current time > start time + 20min
		auto difference = m_fields->endtime - std::chrono::system_clock::now();
		log::debug("{}", difference.count());
		/*
		TODO: REVERSE INEQUALITY TO <= WHEN TESTING COMPLETE
		 */
		if (difference.count() >= 0) {
			// calls the pause function. On windows, Pause (init) is out of line!
			PlayLayer::pauseGame(true);

			log::debug("{}", this->getID());

			// this posts the details of current events
			TimerEvent(true, this).post();
			m_fields->resetTimer();
			
		} else {
			log::debug("Timer not called! {}", difference.count());
		}
	}

	
};

/**
 * Modified editor UI
 * 
 * This is a modified editor UI paged used to track play testing and pausing on the editor. 
 */
#include <Geode/modify/EditorUI.hpp> 
class $modify(EditorUI) {
	struct Fields {
		bool isPlaytesting;
		bool pauseAfterPlaytest;

		// EventListener<EditorTimerTask> timer;
	};

	bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;

		m_fields->pauseAfterPlaytest = false;
		m_fields->isPlaytesting = false;

		// onEvent(); // may cause accesss exception ;)
		return true;
	}

	// sets play testing flag
	void onPlaytest(CCObject* sender) {
		EditorUI::onPlaytest(sender);
		m_fields->isPlaytesting = true;

		m_fields->pauseAfterPlaytest = true; // temp
	}

	// unsets and checks pause layers
	void playtestStopped() {
		EditorUI::playtestStopped();

		if (m_fields->pauseAfterPlaytest) {
			log::debug("x");
			TimerEvent(true, this).post();
		}

		m_fields->isPlaytesting = false;
		
	}

	// pauses when button is pressed, so escape doesnt call 2 EditorPauseLayers
	void onStopPlaytest(cocos2d::CCObject* sender) {
		// this wont cause layering errors hopefully.
		// if (m_fields->isPlaytesting) {
		// 	EditorUI::onPause(this);
		// }
		
			EditorUI::onStopPlaytest(sender);
	}

	// this event happens when binded with the event listener. it will throw a new event when the timer task attatched is called.
	void onEvent() {
		// validates if event shouldnt run due to condition
		if (m_fields->isPlaytesting) {
			m_fields->pauseAfterPlaytest = true;
			return;
		}

		TimerEvent(true, this).post();
	}

	//TODO: impl function
	void resetTimer() {
		return;
	}
};

#include <Geode/modify/EditorPauseLayer.hpp>
class $modify(EditorTimerFix, EditorPauseLayer) {

	bool init(LevelEditorLayer* lvl) {

		if (!EditorPauseLayer::init(lvl)) return false;
		log::debug("INIT!!!");
		// m_fields->listener = {
		// 	this, &onPopupEvent
		// }
			// for (auto node : CCArrayExt<CCNode*>(this->getChildByID("resume-menu")->getChildren())) {
			// 	changeButtonState(node, false);
            // }

		return true;
	}

	static void changeButtonState(CCNode* btn, bool mode) {
		if (auto x = typeinfo_cast<CCMenuItemSpriteExtra*>(btn)) {
			x->setEnabled(mode);
		}
	}

	void onPopupEvent(bool x) {
		for (auto node : CCArrayExt<CCNode*>(this->getChildByID("resume-menu")->getChildren())) {
				changeButtonState(node, x);
            }
	}
};

// this executes the event when it occurs
$execute{
	new EventListener<EventFilter<TimerEvent>>(+[](TimerEvent* ev) {
		if (ev->isActive()) {

			auto layer = ev->getCurrentLayer()->getParent();
			log::debug("{}", layer);
			log::debug("kids {}", layer->getChildrenCount());

			layer = layer->getChildByID("EditorPauseLayer");
			log::debug("is the layer here {}", layer != nullptr);

			/*
			PROBLEM: THIS IS CALLED BEFORE PAUSE LAYER INITIATED
			*/
			if (layer != nullptr && layer->getID() == "EditorPauseLayer") {
				layer->addEventListener<popUpEnabledFilter>(
					[layer](bool mode) {
						for (auto node : CCArrayExt<CCNode*>(layer->getChildByID("resume-menu")->getChildren())) { 
									if (auto nodeAsBtn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
										nodeAsBtn->setEnabled(mode);
									}
								}
							}
						);

				log::debug("Pause Layer Found! Lets activate the event");
				popUpEnabledEvent(true).post();
			}

			std::string timerAlert = fmt::format("Time to take a {} second break!", Mod::get()->getSettingValue<int64_t>("breakTime"));

			auto x = geode::createQuickPopup("Timer", timerAlert, "Start", "Skip", [ev](auto, bool btn2) {
					// yeah I hate this too, don't blame me :)
					if (!btn2) {
						log::debug("{}", btn2);
						// adds a new node to the layer parent
						CCNode* x = CCNode::create(); 
						TimerLayer* timePopUp = TimerLayer::create(fmt::format("{}", Mod::get()->getSettingValue<int64_t>("breakTime")));

						// returns the grandfather of a node, so The popup is placed at the top of the tree
						CCNode* parent = ev->getCurrentLayer(); 

						
						while(parent->getParent() != nullptr) {
							parent = parent->getParent();
							log::debug("parent found!");
						}

						parent->addChild(timePopUp, 20);
						timePopUp->setID("Timer-pop-up"_spr);

						// log::debug("The current layer is {}", curr->getID());

					// 	if (curr->getID() == "Play-Layer") {// TODOFIX THIS ID
					// 	log::debug("Here, we'd add the timerNode!");
					// }

				} else {
					popUpEnabledEvent(false).post();
				}
		}, true);

		

		x->setTouchPriority(-505);

		// log::debug("{}", x->getTouchPriority());
		// x->setID("TimerAlert"_spr);

			return ListenerResult::Stop;

		}
		return ListenerResult::Propagate;
	});
};







// //Handles checking for timer if called

// #include <Geode/modify/PauseLayer.hpp>
// class $modify(MyPauseLayer, PauseLayer) {
// 	struct Fields {
// 		int x;
// 		EventListener<TimerFilter> listener;
// 	};

// 	// this function is meant to bind the listener to the event
// 	void customSetup() override {
// 		PauseLayer::customSetup();
// 		// MyPauseLayer->addEventListener<TimerFilter>
// 	}

// 	// this function occurs on an event
// 	void onEvent() {
// 		// this string SHOULD include the time set in json settings
// 		std::string timerAlert = "Time to take a 20 second break!";

// 		createQuickPopup("Timer", timerAlert, "Start", "Skip", [](bool btn1, auto) {
// 				if (btn1) {
// 					log::debug("Button 1 was pressed");
// 					// this should then allow the custom TimerNode to appear. I dont know how well it will work
// 				}
// 			}
// 		);
// 	}
// };