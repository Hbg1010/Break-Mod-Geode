#pragma once
#include <Geode/Geode.hpp>
#include "../utils/devMode.hpp"
using namespace geode::prelude;

using EditorTimerTask = Task<bool, int>;
EditorTimerTask startEditorTimer(int time);