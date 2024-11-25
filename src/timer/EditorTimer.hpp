#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;


// EditorTimerTask startEditorTimer();

class popUpEnabledEvent : public Event {
public:
    popUpEnabledEvent(bool isEnabled);
    bool mode;

};

class popUpEnabledFilter : public EventFilter<popUpEnabledEvent> {
protected:
    CCNode* m_target;

public:
    using Callback = void(bool);
    popUpEnabledFilter(CCNode* target);
    ListenerResult handle(std::function<Callback> fn, popUpEnabledEvent* ev);

};