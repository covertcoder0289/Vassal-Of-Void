#pragma once
#include "GameState.hpp"
#include "PauseState.hpp"
#include "entities/player/Player.hpp"
#include "world/Tilemap.hpp"
#include <memory>

// Forward declaration to prevent include loops
class StateManager;

class PlayState : public GameState {
public:
    explicit PlayState(StateManager& stateManager);
    ~PlayState() override;

    void update() override;
    void drawTopScreen() override;
    void drawBottomScreen() override;
    void buildTestRoom();

private:    
    Player player;
    bool isPaused = false;
    StateManager& stateManager;
    std::unique_ptr<PauseState> pauseOverlay;
    Tilemap tilemap;
};
