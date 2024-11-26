#include "EditorTimer.hpp"

//This is set up to check for each second, as an early canel would mean this event could just continue existing in memory
EditorTimerTask startEditorTimer(int time) {
    return EditorTimerTask::run([time](auto progress, auto hasBeenCancelled) -> EditorTimerTask::Result {
		log::debug("Starting editor timer!");
        // int time = Mod::get()->getSettingValue<int64_t>("interval") *6; // gets the amount of seconds 

		// sleeps every second, as this needs to check if the event has been cancelled to not cause any problems!
        for (int i = 0; i < time; i++) {
             if (hasBeenCancelled()) {
                progress(time - i); // returns the seconds val before announcing cancel, so it can be stored if timer is paused
                return EditorTimerTask::Cancel();

            } else if (i % 60 == 0) {
				progress(i / 60);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }

		
        return true;
    });
}
