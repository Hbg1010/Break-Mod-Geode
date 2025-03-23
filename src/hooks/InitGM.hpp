// Lowkey idk if i shoulda used saved values via json, but this is how i'll reset it... 
#include <Geode/modify/GameManager.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude

class $modify(InitGM, GameManager) {
    void firstLoad();
}