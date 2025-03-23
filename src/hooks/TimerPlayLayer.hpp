#include <Geode/modify/PlayLayer.hpp>
#include "../timer/TimerEvent.hpp"
#include <chrono>
#include <ctime>

using namespace geode::prelude;

// Controls pausing in game.
class $modify(TimerPlayLayer, PlayLayer) {

	struct Fields {
		std::chrono::time_point<std::chrono::system_clock> endtime;
		bool useTimer;
		bool paused;
		std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> difference;
		
		// resets the timer on call;
		void resetTimer(float time = (float) Mod::get()->getSettingValue<int64_t>("interval")) {
			endtime = std::chrono::system_clock::now() + std::chrono::minutes{time}; // TODO: Replace with setting
		}
	};

	void resetTimer(float time = (float) Mod::get()->getSettingValue<int64_t>("interval"));

	bool init(GJGameLevel* p0, bool p1, bool p2);
	void resetLevel();
	void pauseTimer(bool pauseState);
	void onExit();
};