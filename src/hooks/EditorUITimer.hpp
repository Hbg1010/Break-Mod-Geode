#pragma once

/**
 * Modified editor UI
 * 
 * This is a modified editor UI paged used to track play testing and pausing on the editor. 
 */
#include <Geode/modify/EditorUI.hpp>
#include "../timer/EditorTimer.hpp"
#include "../timer/TimerEvent.hpp"

using namespace geode::prelude;

class $modify(EditorUITimer, EditorUI) {
	struct Fields {
		bool isPlaytesting;
		bool pauseAfterPlaytest;
		bool paused;
		int remainingTime; // this is for keeping the time left after pause

		bool isEditorPaused; //TODO: IMPL

		EventListener<EditorTimerTask> timer;

		// resets timer when it is finished
		bool reset(EditorUITimer* sender, int time) {
			if (this->timer.getFilter().isFinished() || this->timer.getCallback() == nullptr) {
				this->timer.bind(sender, &EditorUITimer::onEvent);
				this->timer.setFilter(startEditorTimer(time)); //TODO ADD 0!!
				return true;
			}
			
			return false;
		}
	};

	// EditorUITimer
	void resetTimer(int time = Mod::get()->getSettingValue<int64_t>("interval") * 6);
	void forceReset(int time = Mod::get()->getSettingValue<int64_t>("interval") * 6);
	void onEvent(EditorTimerTask::Event* ev);
	bool checkEndPlaytest();
	void onUnpause();
	int getRemainder();
	void cancelTimer();
	
	//hooked fns
	bool init(LevelEditorLayer* editorLayer);
	void onPlaytest(CCObject* sender);
	void playtestStopped();
	void onStopPlaytest(CCObject* sender);
	void onPause(CCObject* sender);
	void showUI(bool p0);
};