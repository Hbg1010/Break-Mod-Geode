#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

using countTask = Task<bool, int>;

class TimerLayer : public Popup<int const&> {
public:
    static TimerLayer* create(int const& waitTime);

protected:
    int currentCount; // timer length
    bool countDownOver;
    CCLabelBMFont* timerText; // this is the timer element found on screen. it is updated to represent the timer!
    EventListener<countTask> m_countTaskListener; // this holds the event listener
    
    void countDown(countTask::Event* event);
    bool setup(int const& waitTime);
    void onClick(CCObject* sender);
    virtual void keyBackClicked() override;
    virtual void onClose(cocos2d::CCObject*);
};