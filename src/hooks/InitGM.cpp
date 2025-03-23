#include "InitGM.hpp"

void InitGM::firstLoad() {
    Mod::get()->setSavedValue<float>("savedTime", Mod::get()->getSettingValue<int>("interval"));
    GameManager::firstLoad();
}