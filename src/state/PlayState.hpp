#pragma once
#include "GameState.hpp"
#include "PauseState.hpp"
#include "entities/player/Player.hpp"
#include "entities/enemies/HollowCrawler.hpp"
#include "entities/player/AbilityManager.hpp"
#include "entities/EntityManager.hpp"
#include "world/Tilemap.hpp"
#include "core/Camera.hpp"
#include <memory>

// Forward declaration to prevent include loops
class StateManager;

class PlayState : public GameState {
public:
    explicit PlayState(StateManager& stateManager);
    ~PlayState() override;

    bool init() override;
    void cleanup() override;
    void update() override;
    void drawTopScreen() override;
    void drawBottomScreen() override;
    void buildTestRoom();

private:    
    StateManager& stateManager;
    Tilemap tilemap;
    Player player;
    Camera camera;
    EntityManager entityManager;
    
    bool isPaused = false;
    std::unique_ptr<PauseState> pauseOverlay = nullptr;
    C2D_SpriteSheet spriteSheet = nullptr;
    C2D_Image blockTileImage = {};
};
