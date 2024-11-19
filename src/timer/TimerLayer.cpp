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
    this->setTitle("20 20 20");
    m_closeBtn->setScale(0.5f);
    // m_closeBtn->setPosition(screenSize.width*2, screenSize.height * 3 / 4);

    // this is the 
    timerText = CCLabelBMFont::create("{waitTime} seconds", "bigFont.fnt");
    timerText->setPosition(screenSize / 2);
    this->addChild(timerText);
    timerText->setID("Timer-Text"_spr);

    // this is the extra text above the countdown 
    auto extraText = CCLabelBMFont::create("Look away from your screen for 20 seconds!", "bigFont.fnt");
    extraText->setPosition(screenSize.width/2, screenSize.height/4);
    this->addChild(extraText);
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
        auto spr = ButtonSprite::create("Finish");
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TimerLayer::onClose));
        log::debug("width?!?!{}", spr->m_width);
        // btn->setAnchorPoint({1.f,1.f});
        btn->setPosition(this->getContentWidth()/2, screenSize.height/8);

        // places the new finish button onto the same menu layer as the extra button
        auto parent = (m_closeBtn->getParent());
        parent->addChild(btn);
        btn->setID("finishBtn"_spr);

    // displays time remaining from the task
    } else if (int* progress = event->getProgress()) {
        timerText->setString(fmt::format("{} seconds", *progress).c_str());
    
    // checks if event is cancelled
    } else if (event->isCancelled()) {
        return;
    }


}

void TimerLayer::onClick(CCObject* sender){
    log::debug("clicked!");
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    onClose(sender);   
}

// Avoids access exception by cancelling event onc all
void TimerLayer::onClose(cocos2d::CCObject*) {
    Popup::onClose(this);
    m_countTaskListener.getFilter().cancel();
}


