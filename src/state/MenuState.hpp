// src/states/MenuState.hpp
#pragma once
#include "GameState.hpp"
#include "StateManager.hpp"
#include "PlayState.hpp"
#include <citro2d.h>
#include <vector>
#include <string>
#include <functional>
#include <memory>


class StateManager;
class PlayState;

class MenuState : public GameState {
public:
    explicit MenuState(StateManager& stateManager);
    ~MenuState() override;

    void update() override;
    void drawTopScreen() override;
    void drawBottomScreen() override;

private:

    enum class MenuAction{
        StartGame,
        Options,
        None
    };

    struct MenuItem {
        std::string label;
        MenuAction action;   
    };

    std::vector<MenuItem> items;
    int selectedIndex = 0;

    C2D_TextBuf textBuf;
    C2D_Font font;
    StateManager& stateManager;
};