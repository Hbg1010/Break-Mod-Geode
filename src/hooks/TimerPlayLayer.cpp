#include "TimerPlayLayer.hpp"

/* hooks
========== */

	bool TimerPlayLayer::init(GJGameLevel* p0, bool p1, bool p2){
		if (!PlayLayer::init(p0,p1,p2)) return false;
		// NodeIDs::provideFor(this);
		m_fields->useTimer = Mod::get()->getSettingValue<bool>("playLayer");

		if (m_fields->useTimer) {
			auto tp1 = std::chrono::system_clock::now();

		// m_fields->starttime = std::chrono::system_clock::now();
		m_fields->resetTimer();
		}
		
	
		// std::time_t time = std::chrono::system_clock::to_time_t(m_fields->endtime);
		// log::debug("TIme is {}", std::ctime(&time));
		return true;
	}

    void TimerPlayLayer::resetLevel() {
		PlayLayer::resetLevel();

		if (!m_fields->useTimer) return;

		// condition. checks if current time > start time + 20min
		auto difference = m_fields->endtime - std::chrono::system_clock::now();
		log::debug("{}", difference.count());
		/*
		TODO: REVERSE INEQUALITY TO <= WHEN TESTING COMPLETE
		 */
		if (difference.count() <= 0) {
			// calls the pause function. On windows, Pause (init) is out of line!
			PlayLayer::pauseGame(true);

			log::debug("{}", this->getID());

			// this posts the details of current events
			TimerEvent(true, this).post();
			m_fields->resetTimer();
			
		} else {
			log::debug("Timer not called! {}", difference.count());
		}
	}