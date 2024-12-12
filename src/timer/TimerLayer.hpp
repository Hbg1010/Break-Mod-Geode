#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

using countTask = Task<bool, int>;

class TimerLayer : public Popup<std::string const&> {
public:
    static TimerLayer* create(std::string const& waitTime);

protected:
    void countDown(countTask::Event* event);
    bool setup(std::string const& waitTime);
    void onClick(CCObject* sender);
    virtual void onClose(cocos2d::CCObject*);

};