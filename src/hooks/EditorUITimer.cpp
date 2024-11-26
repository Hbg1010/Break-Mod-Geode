#include "EditorUITimer.hpp"


// resets the timer on call!
void EditorUITimer::resetTimer(int time) {
    if (m_fields->reset(this, time)) {
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

            // if this is paused, this will recieve progress when cancelled
            if (m_fields->paused) {
                m_fields->remainingTime = *progress;

            } else {
			    log::debug("min: {}", *progress);
            }

		} else if (ev->isCancelled()) {
			return;
		}
		// validates if event shouldnt run due to condition
		
	}

    bool EditorUITimer::checkEndPlaytest() {
        if (m_fields->pauseAfterPlaytest) {
            m_fields->pauseAfterPlaytest = false;
            EditorUI::onPause(this);   
		    TimerEvent(true, this).post();
            return true;

        } else {
            return false;
        }
    }

    void onUnpause() {

    }

/* hooks
========== */

bool EditorUITimer::init(LevelEditorLayer* editorLayer) {
    if (!EditorUI::init(editorLayer)) return false;

    m_fields->pauseAfterPlaytest = false;
    m_fields->isPlaytesting = false;
    m_fields->paused = false;

    if (Mod::get()->getSettingValue<bool>("editorLayer")) {
        m_fields->timer.bind(this, &EditorUITimer::onEvent);
        m_fields->timer.setFilter(startEditorTimer(Mod::get()->getSettingValue<int64_t>("interval") * 6));
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
    // if that function doesnt run, normal behavior occurs
    if (!checkEndPlaytest()) {
        EditorUI::onStopPlaytest(sender);
    }
}

void EditorUITimer::onPause(CCObject* sender) {
    if (!checkEndPlaytest()) {
        EditorUI::onPause(sender);
    }
}

void EditorUITimer::showUI(bool p0){
    EditorUI::showUI(p0);

    if (checkEndPlaytest() && m_fields->isPlaytesting){
        CCMenuItemSpriteExtra* stopButton = static_cast<CCMenuItemSpriteExtra*>(this->getChildByID("playtest-menu")->getChildByID("stop-playtest-button"));

        if (stopButton != nullptr) {
            stopButton->setVisible(false);
            stopButton->setEnabled(false);
        } 


        // if (stopButton->getChildByID("stop-playtest-button") != nullptr) {
        //     stopButton->removeChildByID("stop-playtest-button");
        // }
    }
}
