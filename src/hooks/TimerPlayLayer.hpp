#pragma once

#include <Geode/modify/PlayLayer.hpp>
#include "../timer/TimerEvent.hpp"
#include <chrono>
#include <ctime>
#include "../utils/devMode.hpp"

using namespace geode::prelude;

// Controls pausing in game.
class $modify(TimerPlayLayer, PlayLayer) {

	struct Fields {
		std::chrono::time_point<std::chrono::system_clock> endtime;
		bool useTimer;
		bool paused;
		std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> difference;
		
		// resets the timer on call;
		void resetTimer() {
			#ifndef SHORTEN_TIMER_TIME
				endtime = std::chrono::system_clock::now() + std::chrono::minutes{Mod::get()->getSettingValue<int64_t>("interval")};
			#else
				endtime = std::chrono::system_clock::now() + std::chrono::seconds{Mod::get()->getSettingValue<int64_t>("interval")};
			#endif
			
		}
	};

	bool init(GJGameLevel* p0, bool p1, bool p2);
	void resetLevel();
	void pauseTimer(bool pauseState);
	void resetTimer();
};