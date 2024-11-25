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

	bool init(LevelEditorLayer* editorLayer);

	// sets play testing flag
	void onPlaytest(CCObject* sender);
    //  {
	// 	EditorUI::onPlaytest(sender);
	// 	m_fields->isPlaytesting = true;

	// 	// m_fields->pauseAfterPlaytest = true; // temp
	// }

	// unsets and checks pause layers
	void playtestStopped();
    // {
	// 	EditorUI::playtestStopped();

	// 	m_fields->isPlaytesting = false;
		
	// }

	void onPause(CCObject* sender);
    // {
	// 	EditorUI::onPause(sender);

	// 	if (m_fields->pauseAfterPlaytest) {
	// 		TimerEvent(true, this).post();

	// 		m_fields->pauseAfterPlaytest = false;
	// 	}
	// }

	// resets the timer on call!
	void resetTimer();
    // {
	// 	if (m_fields->reset(this)) {
	// 		log::debug("timer was reset!");
	// 	}
	// }

	// cancels the timer before reseting
	void forceReset();
    // {
	// 	m_fields->timer.getFilter().shallowCancel();

	// 	resetTimer();
	// }

	// this event happens when binded with the event listener. it will throw a new event when the timer task attatched is called.
	void onEvent(EditorTimerTask::Event* ev);
    //  {

	// 	if (bool* result = ev->getValue()) {
	// 		if (m_fields->isPlaytesting) {
	// 		m_fields->pauseAfterPlaytest = true;
	// 		return;
	// 	}

	// 	EditorUITimer::onPause(this);
	// 	TimerEvent(true, this).post();


	// 	} else if (int* progress = ev->getProgress()) {
	// 		log::debug("min: {}", *progress);

	// 	} else if (ev->isCancelled()) {
	// 		return;
	// 	}
	// 	// validates if event shouldnt run due to condition
		
	// }
};