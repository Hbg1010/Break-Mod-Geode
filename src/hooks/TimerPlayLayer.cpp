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

void TimerPlayLayer::resetTimer(float time) {
	m_fields->resetTimer(time);
}

/* hooks
========== */

bool TimerPlayLayer::init(GJGameLevel* p0, bool p1, bool p2){
	if (!PlayLayer::init(p0,p1,p2)) return false;
		m_fields->paused = false;
		m_fields->useTimer = Mod::get()->getSettingValue<bool>("playLayer");

		resetTimer(Mod::get()->getSavedValue("savedTime", (float) Mod::get()->getSettingValue<int64_t>("interval")));

	return true;
}

void TimerPlayLayer::resetLevel() {
	PlayLayer::resetLevel();

	//TODO DELTE THIS EW
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

		log::debug("{}", this->getID());

		// this posts the details of current events
		TimerEvent(true, this).post();
		m_fields->resetTimer();
		
	} 
}

// saves value on exit
void TimerPlayLayer::onExit() {
	if (Mod::get()->getSettingValue<bool>("useSaving") && Mod::get()->getSettingValue<bool>("playLayer")) {
		auto nowInMin = std::chrono::time_point_cast<std::chrono::minutes>(std::chrono::system_clock::now());
		double difference = std::chrono::duration_cast<std::chrono::minutes>(m_fields->endtime - nowInMin).count();

		if (difference > 0) Mod::get()->setSavedValue<float>("savedTime", difference);

		// delete &nowInMin; // this may be bad?
	}

	PlayLayer::onExit();
}