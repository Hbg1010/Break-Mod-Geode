#include "EditorUITimer.hpp"

// resets the timer on call!
void EditorUITimer::resetTimer(int time) {
    auto field = m_fields.self();
    if (field->reset(this, time)) {
        field->remainingTime = time; // remaining time resets on force reset as a resault
        #ifdef extraPrints
            log::debug("timer was reset!");
        #endif
    }
}

// cancels the timer before reseting
void EditorUITimer::forceReset(int time) {
    auto field = m_fields.self();
    field->remainingTime = time; // remaining time resets on force reset as a resault
    // log::debug("{}", field->remainingTime);

    if (!field->paused) {
        field->timer.getFilter().cancel();
        field->timer.bind(this, &EditorUITimer::onEvent);
        field->timer.setFilter(startEditorTimer(time)); //TODO ADD 0!!
    }
}

// this event happens when binded with the event listener. it will throw a new event when the timer task attatched is called.
void EditorUITimer::onEvent(EditorTimerTask::Event* ev) {

    if (bool* result = ev->getValue()) {
        if (m_fields->isPlaytesting) {
        m_fields->pauseAfterPlaytest = true;
        return;
        } else {
            timerCall();
        }  
    } else if (int* progress = ev->getProgress()) {
        m_fields->remainingTime = *progress;
    } else if (ev->isCancelled()) {
        #ifdef extraPrints
            log::debug("{}", m_fields->remainingTime);
        #endif
        return;
    }
}

bool EditorUITimer::checkEndPlaytest() {
    if (m_fields->pauseAfterPlaytest) {
        m_fields->pauseAfterPlaytest = false;
        timerCall();
        return true;

    } else {
        return false;
    }
}

void EditorUITimer::timerCall() {
    auto parent = this->getParent()->getParent();

    int childrenNum = parent != nullptr 
    ? parent->getChildrenCount() 
    : 0;
    if (childrenNum > 1) {
        auto children = parent->getChildren();

        for (int i = childrenNum - 1; i >= 0; i--) {
            CCObject* child = children->objectAtIndex(i);
            log::debug("{}", i);
            if (typeinfo_cast<LevelEditorLayer*, CCObject*>(child) == nullptr) children->removeObjectAtIndex(i);
        }
    }
    EditorUI::onPause(this);
    m_fields->remainingTime = Mod::get()->getSettingValue<int64_t>("interval") * timeMult; // this hopefully will fix the problem
    TimerEvent(true, this).post();
}

int EditorUITimer::getRemainder() {
    return m_fields->remainingTime;
}

// pauses the timer
void EditorUITimer::pauseTimer(bool isPaused) {
    m_fields->paused = isPaused;
    if (isPaused) m_fields->timer.getFilter().cancel();
}

bool EditorUITimer::isPaused() {
    return m_fields->paused;
}

void EditorUITimer::cancelTimer() {
    m_fields->timer.getFilter().cancel(); 
}

/* hooks
========== */

bool EditorUITimer::init(LevelEditorLayer* editorLayer) {
    if (!EditorUI::init(editorLayer)) return false;

    auto fields = m_fields.self();
    fields->pauseAfterPlaytest = false;
    fields->isPlaytesting = false;
    
    // sets remaining time from settings
    fields->remainingTime = Mod::get()->getSettingValue<bool>("useSaving")
        ? Mod::get()->getSavedValue<float>("savedTime", Mod::get()->getSettingValue<int64_t>("interval") * timeMult)
        : Mod::get()->getSettingValue<int64_t>("interval") * timeMult;

    // makes sure negatives arent passed, bc like why
    if (fields->remainingTime < 0) {
        int temp = Mod::get()->getSettingValue<int64_t>("interval") * timeMult;
        Mod::get()->getSavedValue<float>("savedTime", temp);
        fields->remainingTime = temp;
    }
    // sets pauses as default before doing anything
    fields->paused = Mod::get()->getSettingValue<bool>("pauseAcrossLevels")
        ? Mod::get()->getSavedValue<bool>("timerPaused", false)
        : false;

    fields->timer.bind(this, &EditorUITimer::onEvent);

    if (Mod::get()->getSettingValue<bool>("editorLayer")) {
        fields->timer.setFilter(
            startEditorTimer(fields->remainingTime)
        );

        // lazy fix ;/
        if (fields->paused) fields->timer.getFilter().cancel();
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
    if (!checkEndPlaytest()) EditorUI::onPause(sender);
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

// // void EditorUITimer::updatePlaybackBtn() {
//     log::debug("updated");
//     EditorUI::updatePlaybackBtn();
// }