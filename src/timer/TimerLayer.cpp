/**
 * Timer Layer: 
 * This pop up appears when a user initates an instance of the timer!
 * When the timer starts, the timer counts down from the setting timer to 0.
 * 
 * This should be initated with a c_str of the mod time
 */

#include "TimerLayer.hpp"

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
TimerLayer* TimerLayer::create(int const& waitTime) {
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
bool TimerLayer::setup(int const& waitTime) {
    currentCount = waitTime;
    this->setTitle("Break Time!");
    m_closeBtn->setScale(0.75f);

    // this is the extra text
    timerText = CCLabelBMFont::create(fmt::format("{}:{} minutes", waitTime/60, waitTime % 60).c_str(), "bigFont.fnt");
    timerText->setPosition({m_mainLayer->getContentWidth()/2,m_mainLayer->getContentHeight()/2});
    m_mainLayer->addChild(timerText);
    timerText->setID("Timer-Text"_spr);

    // this is the extra text above the countdown 
    auto extraText = CCLabelBMFont::create(fmt::format("Look away from your screen for {} seconds!", waitTime).c_str(), "bigFont.fnt");
    extraText->setPosition(m_mainLayer->getContentWidth()/2, m_mainLayer->getContentHeight()/4*3);
    extraText->setScale(.4f);
    m_mainLayer->addChild(extraText);
    extraText->setID("extraText"_spr);

    countDownOver = false;

    // starts the count down here
    m_countTaskListener.bind(this, &TimerLayer::countDown);
    m_countTaskListener.setFilter(beginTimer());
    return true;
}

/**
 * This runs events based around the timer event!
 * 
 * Progress counts down on screen, and the result will be 
 */
void TimerLayer::countDown(countTask::Event* event) {
    
    if (bool* result = event->getValue()) {
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        countDownOver = true;
        auto parent = m_closeBtn->getParent(); // creates the finish button on screen

        auto spr = ButtonSprite::create("Finish");
        auto finishBtn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TimerLayer::onClose));
      
        finishBtn->setPosition(parent->getPositionX(), screenSize.height/8);

        // places the new finish button onto the same menu layer as the extra button
        parent->addChild(finishBtn);
        finishBtn->setID("finishBtn"_spr);

        // plays audio when called
        if (Mod::get()->getSettingValue<bool>("audioAlert")) {
            timerText->setString(fmt::format("Time is up!").c_str());
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
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    onClose(sender);   
}

// Avoids access exception by cancelling event onc all
void TimerLayer::onClose(cocos2d::CCObject*) {
    Popup::onClose(this);
    m_countTaskListener.getFilter().cancel();
}

// cancels an escape input when countdown is over
void TimerLayer::keyBackClicked() {
    if (countDownOver) Popup::keyBackClicked();
}