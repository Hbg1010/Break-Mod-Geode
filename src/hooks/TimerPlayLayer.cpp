#include "TimerPlayLayer.hpp"

// changes the bool to true / false
void TimerPlayLayer::pauseTimer(bool pauseState) {
	auto fieldsPtr = m_fields.self();
	fieldsPtr->paused = pauseState;

	if (fieldsPtr->paused) {
		fieldsPtr->difference = fieldsPtr->endtime - system_clock::now();
		#ifdef extraPrints
		log::debug("{}", fieldsPtr->difference.count());
		#endif
	} else {
		fieldsPtr->endtime = system_clock::now() + fieldsPtr->difference;
	}
}

void TimerPlayLayer::resetTimer(float time) {
	m_fields->resetTimer(time);
}

float TimerPlayLayer::getDefaultTimer() const {
	return Mod::get()->getSettingValue<int64_t>("interval") * 60;
}

// gets the remaining seconds that will be saved
double TimerPlayLayer::remainingSeconds() {
	auto nowInSeconds = time_point_cast<seconds>(system_clock::now());
	return duration_cast<seconds>(!m_fields->paused
		? m_fields->endtime - nowInSeconds
		: m_fields->difference
	).count();
}

/* hooks
========== */

bool TimerPlayLayer::init(GJGameLevel* p0, bool p1, bool p2){
	if (!PlayLayer::init(p0,p1,p2)) return false;
	Mod* modPtr = Mod::get();	// mod is used so much here i may as well just use the pointer
	auto fieldsPtr = m_fields.self();
	fieldsPtr->paused = modPtr->getSettingValue<bool>("pauseAcrossLevels")
		? modPtr->getSavedValue<bool>("timerPaused", false)
		: false;

	fieldsPtr->useTimer = modPtr->getSettingValue<bool>("playLayer");

	#ifdef extraPrints
		log::debug("{}", modPtr->getSavedValue("savedTime", getDefaultTimer()));
	#endif

	// starts the timer

	float startingNum;
	if (modPtr->getSettingValue<bool>("useSaving")) {
		startingNum = modPtr->getSavedValue("savedTime", getDefaultTimer());

		// if timer isn't initiated yet, then this will update the timer based off the current interval
		if (startingNum == -1) {
			startingNum = getDefaultTimer();
			modPtr->setSavedValue<float>("savedTime", getDefaultTimer());
		}

	} else {
		startingNum = getDefaultTimer();
	}

	resetTimer(startingNum);
	fieldsPtr->difference = fieldsPtr->endtime - system_clock::now();

	return true;
}

void TimerPlayLayer::resetLevel() {
	PlayLayer::resetLevel();

	auto fieldsPtr = m_fields.self();

	if (!Mod::get()->getSettingValue<bool>("playLayer") || fieldsPtr->paused) {
		fieldsPtr->useTimer = false;
		return;

	// actually what the fuck was i doing? somehow this makes shit work i guess
	} else if (!fieldsPtr->useTimer) {
		fieldsPtr->difference = fieldsPtr->endtime - system_clock::now();
		fieldsPtr->resetTimer(duration_cast<seconds>(fieldsPtr->difference).count());
		fieldsPtr->useTimer = true;
		return;
	}

	auto difference = fieldsPtr->endtime - system_clock::now();
	
	if (difference.count() <= 0) {
		PlayLayer::pauseGame(true);
		
		#ifdef extraPrints
			log::debug("{}", this->getID());
		#endif
		// this posts the details of current events
		TimerEvent(true, this).post();
		fieldsPtr->resetTimer();
	} 
}

// saves value on exit
void TimerPlayLayer::onExit() {
	if (Mod::get()->getSettingValue<bool>("useSaving") && Mod::get()->getSettingValue<bool>("playLayer")) {
		double toSave = remainingSeconds();

		#ifdef extraPrints
			log::debug("{}", toSave);
		#endif

		if (toSave > 0) {
			Mod::get()->setSavedValue<float>("savedTime", toSave);
		} else Mod::get()->setSavedValue<float>("savedTime", 5); // this is just so the editor (hopefully) wont freak out :)
	}

	PlayLayer::onExit();
}
