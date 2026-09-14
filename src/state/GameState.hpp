//src/state/GameState.hpp
#pragma once

class GameState{
public:
    virtual ~GameState() = default;
    virtual void update() = 0;
    virtual void drawTopScreen() = 0;
    virtual void drawBottomScreen() = 0;
};