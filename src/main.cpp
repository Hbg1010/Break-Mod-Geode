/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
#include <chrono>
#include <ctime>
#include "timer/TimerEvent.cpp"
#include "timer/TimerLayer.cpp"
#include "timer/Layers/TimerSettingsLayer.cpp"

// #include "timer/EditorTimer.cpp"

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
			auto tp1 = std::chrono::system_clock::now();

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
		if (difference.count() <= 0) {
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

#include <Geode/modify/PauseLayer.hpp>
class $modify(TimerPauseLayer, PauseLayer) {
	void customSetup() override {
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
		CCMenuItemSpriteExtra* settingsBtn;

		for (auto* node : CCArrayExt<CCNode*>(settingsMenu->getChildren())) {
			if (node->getID() == "options-button") {
				settingsBtn = static_cast<CCMenuItemSpriteExtra*>(node);
				break;
			}
		}

		if (settingsBtn == nullptr) {
			log::error("Options button could not be found!");
		}

		
		// TimersettingsBtn->setPosition(settingsBtn->getPositionX(), settingsBtn->getPositionY() - settingsBtn->getContentWidth() * 1.5f);
		// TimersettingsBtn->setContentSize(settingsBtn->getScaledContentSize());
		settingsMenu->addChild(TimersettingsBtn);
		TimersettingsBtn->setID("timer-settings-btn"_spr);

		settingsMenu->updateLayout();
	}

	void onTimerSettings(CCObject* sender) {
		log::debug("Settings menu was pressed");
		auto layer = TimerSettingsLayer::create(this->getID());
		layer->m_scene = this;
		layer->show();
	}
};




using EditorTimerTask = Task<bool, int>;

//This is set up to check for each second, as an early canel would mean this event could just continue existing in memory
EditorTimerTask startEditorTimer() {
    return EditorTimerTask::run([](auto progress, auto hasBeenCancelled) -> EditorTimerTask::Result {
		log::debug("Starting editor timer!");
        int time = Mod::get()->getSettingValue<int64_t>("interval") *60; // gets the amount of seconds 

		// sleeps every second, as this needs to check if the event has been cancelled to not cause any problems!
        for (int i = 0; i < time; i++) {
             if (hasBeenCancelled()) {
                return EditorTimerTask::Cancel();

            } else if (i % 60 == 0) {
				progress(i / 60);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }

		
        return true;
    });
}

/**
 * Modified editor UI
 * 
 * This is a modified editor UI paged used to track play testing and pausing on the editor. 
 */
#include <Geode/modify/EditorUI.hpp> 
class $modify(EditorUITimer, EditorUI) {
	struct Fields {
		bool isPlaytesting;
		bool pauseAfterPlaytest;
		EventListener<EditorTimerTask> timer;

		// resets timer when it is finished
		bool reset(EditorUITimer* sender) {
			if (this->timer.getFilter().isFinished()) {
				this->timer.bind(sender, &EditorUITimer::onEvent);
				this->timer.setFilter(startEditorTimer());
				return true;
			}
			
			return false;
		}

	};

	bool init(LevelEditorLayer* editorLayer) {
		if (!EditorUI::init(editorLayer)) return false;

		m_fields->pauseAfterPlaytest = false;
		m_fields->isPlaytesting = false;
		m_fields->isPlaytesting = false;
		
		if (Mod::get()->getSettingValue<bool>("editorLayer")) {
			m_fields->timer.bind(this, &EditorUITimer::onEvent);
			m_fields->timer.setFilter(startEditorTimer());
		}
		
		

		return true;
	}

	// sets play testing flag
	void onPlaytest(CCObject* sender) {
		EditorUI::onPlaytest(sender);
		m_fields->isPlaytesting = true;

		// m_fields->pauseAfterPlaytest = true; // temp
	}

	// unsets and checks pause layers
	void playtestStopped() {
		EditorUI::playtestStopped();

		m_fields->isPlaytesting = false;
		
	}

	void onPause(CCObject* sender) {
		EditorUI::onPause(sender);

		if (m_fields->pauseAfterPlaytest) {
			TimerEvent(true, this).post();

			m_fields->pauseAfterPlaytest = false;
		}
	}

	// resets the timer on call!
	void resetTimer() {
		if (m_fields->reset(this)) {
			log::debug("timer was reset!");
		}
	}

	// this event happens when binded with the event listener. it will throw a new event when the timer task attatched is called.
	void onEvent(EditorTimerTask::Event* ev) {

		if (bool* result = ev->getValue()) {
			if (m_fields->isPlaytesting) {
			m_fields->pauseAfterPlaytest = true;
			return;
		}

		EditorUITimer::onPause(this);
		TimerEvent(true, this).post();


		} else if (int* progress = ev->getProgress()) {
			log::debug("min: {}", *progress);

		} else if (ev->isCancelled()) {
			return;
		}
		// validates if event shouldnt run due to condition
		
	}

	// //TODO: impl function
	// void resetTimer() {
	// 	return;
	// }
};

#include <Geode/modify/EditorPauseLayer.hpp>
class $modify(EditorTimerFix, EditorPauseLayer) {

	// bool init(LevelEditorLayer* lvl) {

	// 	if (!EditorPauseLayer::init(lvl)) return false;
	// 	// m_fields->listener = {
	// 	// 	this, &onPopupEvent
	// 	// }
	// 		// for (auto node : CCArrayExt<CCNode*>(this->getChildByID("resume-menu")->getChildren())) {
	// 		// 	changeButtonState(node, false);
    //         // }

	// 	return true;
	// }

	// reseys the timer found in EditorUITimer
	void onResume(CCObject* sender) {
		EditorPauseLayer::onResume(sender);
		auto x = static_cast<EditorUITimer*>(EditorUITimer::get());

		x->resetTimer();

	}
};

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

						parent->addChild(timePopUp, 20);
						timePopUp->setID("Timer-pop-up"_spr);
				} 
		}, true);

		// if in editor, this will set the touch priority above the EditorPauseLayer
		// very open to knowing a better way to do this! ! ! Unlike the editorUI though, theres no get function :(
		auto editorPauseLayer = ev->getCurrentLayer()->getParent()->getChildByID("EditorPauseLayer");
		if (editorPauseLayer != nullptr) x->setTouchPriority(static_cast<EditorPauseLayer*>(editorPauseLayer)->getTouchPriority() - 1);
		
		// log::debug("{}", x->getTouchPriority());
		// x->setID("TimerAlert"_spr);

			return ListenerResult::Stop;

		}
		return ListenerResult::Propagate;
	});
};