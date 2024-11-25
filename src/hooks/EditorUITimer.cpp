#include "EditorUITimer.hpp"


// resets the timer on call!
void EditorUITimer::resetTimer() {
    if (m_fields->reset(this)) {
        log::debug("timer was reset!");
    }
}

// cancels the timer before reseting
void EditorUITimer::forceReset() {
    m_fields->timer.getFilter().shallowCancel();

    resetTimer();
}

// this event happens when binded with the event listener. it will throw a new event when the timer task attatched is called.
	void EditorUITimer::onEvent(EditorTimerTask::Event* ev) {

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

/* hooks
========== */

bool EditorUITimer::init(LevelEditorLayer* editorLayer) {
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
void EditorUITimer::onPlaytest(CCObject* sender) {
    EditorUI::onPlaytest(sender);
    m_fields->isPlaytesting = true;

    // m_fields->pauseAfterPlaytest = true; // temp
}

// unsets and checks pause layers
void EditorUITimer::playtestStopped() {
    EditorUI::playtestStopped();

    m_fields->isPlaytesting = false;
}

void EditorUITimer::onStopPlaytest(CCObject* sender) {
    if (m_fields->pauseAfterPlaytest) {
        EditorUITimer::onPause(sender);

    } else {
        EditorUI::onStopPlaytest(sender);
    }
}

void EditorUITimer::onPause(CCObject* sender) {
    EditorUI::onPause(sender);

    if (m_fields->pauseAfterPlaytest) {
        TimerEvent(true, this).post();

        m_fields->pauseAfterPlaytest = false;
    }
}
