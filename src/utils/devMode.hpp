#pragma once

static class devMode
{
private:
    bool isEnabled;
public:
    devMode(bool enabled);
    ~devMode();
};

devMode::devMode(bool enabled) {
    isEnabled = enabled;
}

devMode::~devMode()
{
}
