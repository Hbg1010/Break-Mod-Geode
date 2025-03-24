#include "EditorTimer.hpp"

//This is set up to check for each second
EditorTimerTask startEditorTimer(int time) {
    return EditorTimerTask::run([time](auto progress, auto hasBeenCancelled) -> EditorTimerTask::Result {
		// log::debug("Starting editor timer!");

		// sleeps every second, as this needs to check if the event has been cancelled to not cause any problems!
        for (int i = 0; i < time; i++) {
             if (hasBeenCancelled()) {
                return EditorTimerTask::Cancel();
            }
            // log::debug("{}", time - i);
            progress(time - i);
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }

        return true;
    });
}
