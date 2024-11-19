#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class TimerEvent : public Event {
    	public:
		TimerEvent(bool active, CCNode* currentLayer);
		bool isActive();
		CCNode* getCurrentLayer();

		protected:
			bool m_active;
};

class TimerFilter : public EventFilter<TimerEvent> {
	public:
		using Callback = void();

		ListenerResult handle(std::function<Callback> fn, TimerEvent* ev);
};
