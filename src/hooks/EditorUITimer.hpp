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

	// EditorUITimer
	void resetTimer();
	void forceReset();
	void onEvent(EditorTimerTask::Event* ev);
	bool checkEndPlaytest();

	//hooked fns
	bool init(LevelEditorLayer* editorLayer);
	void onPlaytest(CCObject* sender);
	void playtestStopped();
	void onStopPlaytest(CCObject* sender);
	void onPause(CCObject* sender);
	void showUI(bool p0);
};