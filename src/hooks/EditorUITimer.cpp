#include "EditorUITimer.hpp"

// resets the timer on call!
void EditorUITimer::resetTimer(int time) {
    if (m_fields->reset(this, time)) {
        m_fields->remainingTime = time; // remaining time resets on force reset as a resault
        log::debug("timer was reset!");
    }
}

// cancels the timer before reseting
void EditorUITimer::forceReset(int time) {
    m_fields->remainingTime = time; // remaining time resets on force reset as a resault
    log::debug("{}", m_fields->remainingTime);

    if (!m_fields->paused) {
        m_fields->timer.getFilter().cancel();
        m_fields->timer.bind(this, &EditorUITimer::onEvent);
        m_fields->timer.setFilter(startEditorTimer(time)); //TODO ADD 0!!
    }
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
            m_fields->remainingTime = *progress;
            // if (m_fields->remainingTime % 60 == 0) log::debug("{}", m_fields->remainingTime);

		} else if (ev->isCancelled()) {
            // m_fields->remainingTime = *ev->getProgress();
            log::debug("{}", m_fields->remainingTime);
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

int EditorUITimer::getRemainder() {
    return m_fields->remainingTime;
}

void EditorUITimer::pauseTimer(bool isPaused) {
    m_fields->paused = isPaused;

    if (isPaused) {
            m_fields->timer.getFilter().cancel();

        // } else {
        //     log::debug("{}", m_fields->remainingTime);
        //     resetTimer(m_fields->remainingTime);
        }
}

bool EditorUITimer::isPaused() {
    return m_fields->paused;
}

/* hooks
========== */

bool EditorUITimer::init(LevelEditorLayer* editorLayer) {
    if (!EditorUI::init(editorLayer)) return false;

    auto fields = m_fields.self();
    fields->pauseAfterPlaytest = false;
    fields->isPlaytesting = false;
    fields->paused = false;

    fields->timer.bind(this, &EditorUITimer::onEvent);

    if (Mod::get()->getSettingValue<bool>("editorLayer")) {
        fields->timer.setFilter(startEditorTimer(Mod::get()->getSettingValue<int64_t>("interval") * 60));
    }
    
    return true;
}

// sets play testing flag
void EditorUITimer::onPlaytest(CCObject* sender) {
    EditorUI::onPlaytest(sender);
    m_fields->isPlaytesting = true;
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

// void EditorUITimer::destructor() {
//     if (Mod::get()->getSettingValue<bool>("useSaving") && Mod::get()->getSettingValue<bool>("editorLayer")) {
//         if ()
//         Mod::get()->setSettingValue<float>("savedTime", (float) m_fields->remainingTime);
// 	}
//     EditorUI::~CCDestructor();
// }
