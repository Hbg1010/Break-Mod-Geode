#include "TimerPlayLayer.hpp"

// changes the bool to true / false
void TimerPlayLayer::pauseTimer(bool pauseState) {
	// log::debug("paused");

	m_fields->paused = pauseState;

	if (m_fields->paused) {
		m_fields->difference = m_fields->endtime - std::chrono::system_clock::now();
		#ifdef extraPrints
		log::debug("{}", m_fields->difference.count());
		#endif
	} else {
		m_fields->endtime = std::chrono::system_clock::now() + m_fields->difference;
	}
}

void TimerPlayLayer::resetTimer(float time) {
	m_fields->resetTimer(time);
}

float TimerPlayLayer::getDefaultTimer() const {
	return Mod::get()->getSettingValue<int64_t>("interval") * 60;
}

/* hooks
========== */

bool TimerPlayLayer::init(GJGameLevel* p0, bool p1, bool p2){
	if (!PlayLayer::init(p0,p1,p2)) return false;
		m_fields->paused = false;
		m_fields->useTimer = Mod::get()->getSettingValue<bool>("playLayer");

		#ifdef extraPrints
		log::debug("{}", Mod::get()->getSavedValue("savedTime", getDefaultTimer()));
		#endif

		// resets the timer
		resetTimer(Mod::get()->getSettingValue<bool>("useSaving") ? 
			Mod::get()->getSavedValue("savedTime", getDefaultTimer()) :
			getDefaultTimer()
		);

	return true;
}

void TimerPlayLayer::resetLevel() {
	PlayLayer::resetLevel();

	//TODO DELTE THIS EW
	if (!Mod::get()->getSettingValue<bool>("playLayer") || m_fields->paused) {
		m_fields->useTimer = false;
		return;

	// actually what the fuck was i doing?
	} else if (!m_fields->useTimer) {
		m_fields->resetTimer();
		m_fields->useTimer = true;
		return;
	}

	auto difference = m_fields->endtime - std::chrono::system_clock::now();
	
	if (difference.count() <= 0) {
		PlayLayer::pauseGame(true);
		
		#ifdef extraPrints
			log::debug("{}", this->getID());
		#endif
		// this posts the details of current events
		TimerEvent(true, this).post();
		m_fields->resetTimer();
	} 
}

// saves value on exit
void TimerPlayLayer::onExit() {
	if (Mod::get()->getSettingValue<bool>("useSaving") && Mod::get()->getSettingValue<bool>("playLayer")) {
		auto nowInSeconds = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
		double difference = std::chrono::duration_cast<std::chrono::seconds>(m_fields->endtime - nowInSeconds).count();

		if (difference > 0) Mod::get()->setSavedValue<float>("savedTime", difference);

		log::debug("{}", Mod::get()->getSavedValue<float>("savedTime"));
	}

	PlayLayer::onExit();
}
