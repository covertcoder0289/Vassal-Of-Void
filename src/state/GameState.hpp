//src/state/GameState.hpp
#pragma once

class GameState{
public:
    virtual ~GameState() = default;
    virtual bool init() { return true; }
    virtual void cleanup() {}
    virtual void update() = 0;
    virtual void drawTopScreen() = 0;
    virtual void drawBottomScreen() = 0;
};