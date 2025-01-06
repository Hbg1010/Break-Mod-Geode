/**
 * Timer Layer: 
 * This pop up appears when a user initates an instance of the timer!
 * When the timer starts, the timer counts down from the setting timer to 0.
 * 
 * This should be initated with a c_str of the mod time
 */

#include "TimerLayer.hpp"

int currentCount; // timer length
CCLabelBMFont* timerText; // this is the timer element found on screen. it is updated to represent the timer!
EventListener<countTask> m_countTaskListener; // this holds the event listener

// This tasks controls the countdown and messages using a seperate thread.
countTask beginTimer() {
    return countTask::run([](auto progress, auto hasBeenCancelled) -> countTask::Result {
        // gets the seconds time
        int secondsVal = Mod::get()->getSettingValue<int64_t>("breakTime");  

        // holds return value
        bool x = false;

        for (int i = secondsVal - 1; i >= 0; i--) {
            if (hasBeenCancelled()) {
                return countTask::Cancel();
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));

            progress(i); // returns the timer progress

            if (secondsVal == 0) {
                x = true;
            }
        }    

        return x;

    }, "counts down using seperate thread to sleep on thread and return propper values");
}
// creates the function
TimerLayer* TimerLayer::create(std::string const& waitTime) {
    auto temp = new TimerLayer();

    // trys to make node
    if (temp->initAnchored(368, 280, waitTime)) {
        temp->autorelease();
        return temp;

    } else {
        CC_SAFE_DELETE(temp);

        return nullptr;
    }
}

/**
 * Initiates the popup
 */
bool TimerLayer::setup(std::string const& waitTime) {
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    currentCount = std::stoi(waitTime);
    this->setTitle("Break Time!");
    m_closeBtn->setScale(0.5f);
    // m_closeBtn->setPosition(screenSize.width*2, screenSize.height * 3 / 4);

    // this is the extra text
    timerText = CCLabelBMFont::create(fmt::format("{} seconds", waitTime).c_str(), "bigFont.fnt");
    timerText->setPosition({m_mainLayer->getContentWidth()/2,m_mainLayer->getContentHeight()/2});
    m_mainLayer->addChild(timerText);
    timerText->setID("Timer-Text"_spr);

    // this is the extra text above the countdown 
    auto extraText = CCLabelBMFont::create(fmt::format("Look away from your screen for {} seconds!", waitTime).c_str(), "bigFont.fnt");
    extraText->setPosition(m_mainLayer->getContentWidth()/2, m_mainLayer->getContentHeight() /4*3);
    extraText->setScale(.4f);
    m_mainLayer->addChild(extraText);
    extraText->setID("extraText"_spr);

    // starts the count down here
    m_countTaskListener.bind(this, &TimerLayer::countDown);
    m_countTaskListener.setFilter(beginTimer());
    return true;
}

/**
 * This runs events based around the timer event!
 * 
 * Progress counts down on screen, and the resault will be 
 */
void TimerLayer::countDown(countTask::Event* event) {
    
    if (bool* resault = event->getValue()) {
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

        // creates the finish button on screen
        auto parent = (m_closeBtn->getParent());
        
        auto spr = ButtonSprite::create("Finish");
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TimerLayer::onClose));
      
        // btn->setAnchorPoint({.5f,.5f});
        btn->setPosition(parent->getPositionX(), screenSize.height/8);

        // places the new finish button onto the same menu layer as the extra button
        parent->addChild(btn);
        btn->setID("finishBtn"_spr);

        // plays audio when called
        if (Mod::get()->getSettingValue<bool>("audioAlert")) {
            timerText->setString(fmt::format("Time is up!").c_str());
            log::debug("sound here! {} volume", GameManager::get()->m_sfxVolume);
            // FMODAudioEngine::sharedEngine()->playEffect()
            // im using the 2nd channel (if thats what int p2 is) to allow sounds on PauseLayer 
            // FMODAudioEngine::sharedEngine()->
            FMODAudioEngine::sharedEngine()->resumeAllAudio();
            FMODAudioEngine::sharedEngine()->playEffectAdvanced("reward01.ogg", 1, 0, GameManager::get()->m_sfxVolume, 1, true, false, 0, 0, 0, 0, false, 0, false, true, 0, 0, 0, 0);
        }

    // displays time remaining from the task
    } else if (int* progress = event->getProgress()) {
        timerText->setString(fmt::format("{} seconds", *progress).c_str());
    
    // checks if event is cancelled
    } else if (event->isCancelled()) {
        return;
    }


}
void TimerLayer::onClick(CCObject* sender){
    // log::debug("clicked!");
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    onClose(sender);   
}

// Avoids access exception by cancelling event onc all
void TimerLayer::onClose(cocos2d::CCObject*) {
    Popup::onClose(this);
    m_countTaskListener.getFilter().cancel();
}