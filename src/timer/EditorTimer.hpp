#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

using EditorTimerTask = Task<bool, bool>;

EditorTimerTask startEditorTimer();