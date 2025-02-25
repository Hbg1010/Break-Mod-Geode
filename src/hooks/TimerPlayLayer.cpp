#include "TimerPlayLayer.hpp"

// changes the bool to true / false
void TimerPlayLayer::pauseTimer(bool pauseState) {
	// log::debug("paused");

	m_fields->paused = pauseState;

	if (m_fields->paused) {
		m_fields->difference = m_fields->endtime - std::chrono::system_clock::now();
		log::debug("{}", m_fields->difference.count());
		
	} else {
		m_fields->endtime = std::chrono::system_clock::now() + m_fields->difference;
	}
}

void TimerPlayLayer::resetTimer() {
	m_fields->resetTimer();
}

/* hooks
========== */

	bool TimerPlayLayer::init(GJGameLevel* p0, bool p1, bool p2){
		if (!PlayLayer::init(p0,p1,p2)) return false;
			m_fields->paused = false;
			m_fields->useTimer = Mod::get()->getSettingValue<bool>("playLayer");

			resetTimer();

		return true;
	}

    void TimerPlayLayer::resetLevel() {
		PlayLayer::resetLevel();

		if (!Mod::get()->getSettingValue<bool>("playLayer") || m_fields->paused) {
			m_fields->useTimer = false;
			return;

		} else if (!m_fields->useTimer) {
			m_fields->resetTimer();
			m_fields->useTimer = true;
		}

		auto difference = m_fields->endtime - std::chrono::system_clock::now();
		// log::debug("{}", difference.count());
		
		if (difference.count() <= 0) {
			PlayLayer::pauseGame(true);

			// this posts the details of current events
			TimerEvent(true, this).post();
			m_fields->resetTimer();
			
		} 
	}