// src/states/PauseState.hpp
#pragma once
#include "GameState.hpp"
#include <citro2d.h>
#include <vector>
#include <string>
#include <functional>
#include <memory>

class StateManager; // Forward declaration

class PauseState : public GameState {
public:
    // FIX: Takes StateManager reference and a simple resume callback
    explicit PauseState(StateManager& stateManager, std::function<void()> onResumeFn);
    ~PauseState() override;

    void update() override;
    void drawTopScreen() override;
    void drawBottomScreen() override;

private:
    enum class MenuAction {
        Continue,
        Options,
        QuitToMenu
    };

    struct MenuItem {
        std::string label;
        MenuAction action; // FIX: Enum action instead of a nested lambda
    };

    std::vector<MenuItem> items;
    int selectedIndex = 0;

    C2D_TextBuf textBuf;
    C2D_Font font;
    StateManager& stateManager;
    std::function<void()> onResumeFn;
}; 
