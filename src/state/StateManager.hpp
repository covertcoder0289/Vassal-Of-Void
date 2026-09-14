//src/state/StateManager.cpp

#pragma once
#include "GameState.hpp"
#include <memory>

class StateManager {
public:
    void setState(std::unique_ptr<GameState> newState) {
        pending = std::move(newState);
    }

    void update() {
        if (pending) {
            current = std::move(pending); // old state destroyed here
        }
        if (current) current->update();
    }

    void drawTopScreen()    { if (current) current->drawTopScreen(); }
    void drawBottomScreen() { if (current) current->drawBottomScreen(); }

private:
    std::unique_ptr<GameState> current;
    std::unique_ptr<GameState> pending;
};