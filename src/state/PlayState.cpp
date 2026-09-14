#include "PlayState.hpp"
#include "MenuState.hpp"
#include "StateManager.hpp"
//#include "../physics/CollisionWorld.hpp"
#include <3ds.h>

PlayState::PlayState(StateManager& stateManager) 
    : stateManager(stateManager) {
    buildTestRoom();

    pauseOverlay = std::make_unique<PauseState>(stateManager, [this]() { isPaused = false; });
}

PlayState::~PlayState() = default;

void PlayState::buildTestRoom() {
    constexpr int COLS = 25;
    constexpr int ROWS = 15;

    // 0 = Air, 1 = Ground (Brown), 2 = Platform (Yellow)
    const std::vector<int> roomData = {
        // Rows 0 to 8: Empty Air
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        
        // Row 7 & 9: Floating Platforms (Yellow)
        0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,

        // Row 13 & 14: Floor Ground with a 3-tile Pit Gap in the middle
        1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1
    };

    tilemap.loadMap(COLS, ROWS, roomData);
}

void PlayState::update() {
    u32 kDown = hidKeysDown();

    if (kDown & KEY_START) {
        isPaused = !isPaused;
    }

    if (isPaused) {
        pauseOverlay->update();
    } else {
        //scanner.update();
        player.update(tilemap);
        //CollisionWorld::resolvePlayerCollisions(player, tilemap);
    }
}

void PlayState::drawTopScreen() {
    tilemap.draw();
    player.draw();

    if (isPaused) {
        C2D_DrawRectSolid(0.0f, 0.0f, 0.2f, 400.0f, 240.0f, C2D_Color32(0, 0, 0, 180));
        pauseOverlay->drawTopScreen();
    }
}

void PlayState::drawBottomScreen() {
    if (isPaused) {
        pauseOverlay->drawBottomScreen();
    } else {
        //scanner.draw();
    }
}
