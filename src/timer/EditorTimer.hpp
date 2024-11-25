#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

using EditorTimerTask = Task<bool, int>;
EditorTimerTask startEditorTimer();