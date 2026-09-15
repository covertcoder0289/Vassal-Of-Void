//src/state/StateManager.cpp
#pragma once
#include "GameState.hpp"
#include <memory>
#include <citro2d.h>
#include <3ds.h>

class StateManager {
public:
    StateManager() = default;
    ~StateManager() = default;

    void setState(std::unique_ptr<GameState> newState);
    void update();
    void drawTopScreen();
    void drawBottomScreen();

private:
    std::unique_ptr<GameState> current;
    std::unique_ptr<GameState> pending;
};