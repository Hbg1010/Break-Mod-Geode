#include "EditorTimer.hpp"

//This is set up to check for each second, as an early canel would mean this event could just continue existing in memory
EditorTimerTask startEditorTimer() {
    return EditorTimerTask::run([](auto progress, auto hasBeenCancelled) -> EditorTimerTask::Result {
        int time = Mod::get()->getSettingValue<int64_t>("interval") *60; // gets the amount of seconds 

        for (int i = 0; i < time; i++) {
             if (hasBeenCancelled()) {
                return countTask::Cancel();
            } else if (i % 60 = 0) {
                log::debug("minute {}", i / 60);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }

        return true;
    });
}