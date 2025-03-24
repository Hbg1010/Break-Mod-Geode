#pragma once

#include <Geode/modify/PlayLayer.hpp>
#include "../timer/TimerEvent.hpp"
#include <chrono>
#include <ctime>
#include "../utils/devMode.hpp"

using namespace geode::prelude;
using namespace std::chrono;
// Controls pausing in game.
class $modify(TimerPlayLayer, PlayLayer) {

	struct Fields {
		std::chrono::time_point<std::chrono::system_clock> endtime;
		bool useTimer;
		bool paused;
		std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> difference;
		
		// resets the timer on call;
		void resetTimer(float time = (float) Mod::get()->getSettingValue<int64_t>("interval") * 60) {
			#ifndef SHORTEN_TIMER_TIME
				endtime = std::chrono::system_clock::now() + std::chrono::seconds{(int) time};
			#else
				endtime = std::chrono::system_clock::now() + std::chrono::seconds{(int) time / 60};
			#endif
		}
	};

	void pauseTimer(bool pauseState);
	void resetTimer(float time = (float) Mod::get()->getSettingValue<int64_t>("interval") * 60);
	float getDefaultTimer() const;
	double remainingSeconds();
	
	//hooked fns
	bool init(GJGameLevel* p0, bool p1, bool p2);
	void resetLevel();
	void onExit();

};