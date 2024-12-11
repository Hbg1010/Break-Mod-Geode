#include "EditorTimer.hpp"

//This is set up to check for each second, as an early canel would mean this event could just continue existing in memory
EditorTimerTask startEditorTimer(int time) {
    return EditorTimerTask::run([time](auto progress, auto hasBeenCancelled) -> EditorTimerTask::Result {
		log::debug("Starting editor timer!");
        // int time = Mod::get()->getSettingValue<int64_t>("interval") *6; // gets the amount of seconds 

		// sleeps every second, as this needs to check if the event has been cancelled to not cause any problems!
        for (int i = 0; i < time; i++) {
             if (hasBeenCancelled()) {
                return EditorTimerTask::Cancel();
            }
            log::debug("{}", time - i);
            progress(time - i);
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }

		
        return true;
    });
}
