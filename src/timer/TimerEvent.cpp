#include "TimerEvent.hpp"

bool m_active;
CCNode* m_current;

TimerEvent::TimerEvent(bool active, CCNode* currentLayer) {
    m_active = active;
    m_current = currentLayer;

    // log::debug("{}",m_current->getID());
};

bool TimerEvent::isActive(){
    return m_active;
};

CCNode* TimerEvent::getCurrentLayer() {
    if (m_current == nullptr) {
        log::error("Nullptr called from getCurrentLayer()");
    }

    return m_current;
}

ListenerResult TimerFilter::handle(std::function<Callback> fn, TimerEvent* ev) {
    fn();
    return ListenerResult::Stop;
};