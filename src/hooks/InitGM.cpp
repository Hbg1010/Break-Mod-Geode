#include "InitGM.hpp"
#ifdef extraPrints
    #include <Geode/loader/Log.hpp>
#endif

void InitGM::firstLoad() {
    Mod::get()->setSavedValue<float>("savedTime", Mod::get()->getSettingValue<int>("interval"));
    #ifdef extraPrints
        log::debug("settins reset!");
    #endif
    GameManager::firstLoad();
}