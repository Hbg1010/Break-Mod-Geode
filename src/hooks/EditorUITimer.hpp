#pragma once

/**
 * Modified editor UI
 * 
 * This is a modified editor UI paged used to track play testing and pausing on the editor. 
 */
#include <Geode/modify/EditorUI.hpp>
#include "../timer/EditorTimer.hpp"
#include "../timer/TimerEvent.hpp"
#include "../utils/devMode.hpp"

#ifndef SHORTEN_TIMER_TIME
	#define timeMult 60
#else
	#define timeMult 6
#endif

using namespace geode::prelude;

class $modify(EditorUITimer, EditorUI) {
	struct Fields {
		bool isPlaytesting;
		bool pauseAfterPlaytest; // tells the game to pause once play testing is completed
		bool paused;
		int remainingTime; // this is for keeping the time left after pause

		bool isEditorPaused; //TODO: IMPL

		EventListener<EditorTimerTask> timer;

		// resets timer when it is finished
		bool reset(EditorUITimer* sender, int time) {
			if (this->timer.getFilter().isFinished() || this->timer.getFilter().isCancelled()) {
				#ifdef extraPrints
					log::debug("{}", time);
				#endif
				this->timer.disable();
				this->timer.setFilter(startEditorTimer(time)); //TODO ADD 0!!
				this->timer.enable();
				return true;
			}
			
			return false;
		}
	};

	// EditorUITimer
	void resetTimer(int time = Mod::get()->getSettingValue<int64_t>("interval") * timeMult);
	void forceReset(int time = Mod::get()->getSettingValue<int64_t>("interval") * timeMult);
	void onEvent(EditorTimerTask::Event* ev);
	bool checkEndPlaytest();
	void onUnpause();
	int getRemainder();
	void pauseTimer(bool isPaused);
	bool isPaused();
	void cancelTimer();
	
	//hooked fns
	bool init(LevelEditorLayer* editorLayer);
	void onPlaytest(CCObject* sender);
	void playtestStopped();
	void onStopPlaytest(CCObject* sender);
	void onPause(CCObject* sender);
	void showUI(bool p0);
};