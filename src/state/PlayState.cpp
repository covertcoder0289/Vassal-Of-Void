//src/state/PlayState.cpp
#include "PlayState.hpp"
#include "MenuState.hpp"
#include "StateManager.hpp"
//#include "../physics/CollisionWorld.hpp"
#include <3ds.h>
#include "sprites.h"
#include "world/RoomLoader.hpp"
#include "core/InputMap.hpp"

PlayState::PlayState(StateManager& stateManager) 
    : stateManager(stateManager) {
    buildTestRoom();

    pauseOverlay = std::make_unique<PauseState>(stateManager, [this]() { isPaused = false; });
}

PlayState::~PlayState()
{  
    cleanup();
}

bool PlayState::init() {
    // Load the compiled t3x spritesheet from RomFS or embedded memory
    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (!spriteSheet) {
        //svcBreak(USERBREAK_PANIC);
        return false; // Failed to load texture sheet
    }

    // Retrieve the image handle for your custom 16x16 block
    blockTileImage = C2D_SpriteSheetGetImage(spriteSheet, sprites_idx);

    // Pass the tile image handle to the tilemap so it knows what to render
    tilemap.setTileTexture(blockTileImage);

    return true;
}

void PlayState::cleanup() {
    // Free VRAM/RAM taken up by the sprite sheet
    if (spriteSheet) {
        C2D_SpriteSheetFree(spriteSheet);
        spriteSheet = nullptr;
    }
}

void PlayState::buildTestRoom() {
    int cols, rows;
    std::vector<int> roomData;
    if (!RoomLoader::loadFromFile("romfs:/rooms/test_room.json", cols, rows, roomData)) {
        svcBreak(USERBREAK_PANIC); // fail loud, same instinct as the sprite-loading bug
    }
    tilemap.loadMap(cols, rows, roomData);
    float spawnX, spawnY;
    if (RoomLoader::loadSpawnPoint("romfs:/rooms/test_room.json", spawnX, spawnY)) {
        player.setPosition(spawnX, spawnY);
    }
}
Camera camera(400.0f, 240.0f); // Top screen resolution
void PlayState::update() {
    u32 kDown = hidKeysDown();
    u32 kHeld = hidKeysHeld();

    float targetX = player.getX() + 8.0f;
    float targetY = player.getY() + 8.0f;
    float mapWidthPx = tilemap.getColumns() * 16.0f;
    float mapHeightPx = tilemap.getRows() * 16.0f;

    //look up and down if player is on the ground
    if(kHeld & KEY_UP && player.getOnGround()) 
        targetY -=90.0f;
    if(kHeld & KEY_DOWN && player.getOnGround()) 
        targetY +=90.0f; 

    if (kDown & KEY_START) {
        isPaused = !isPaused;
    }

    if (isPaused) {
        pauseOverlay->update();
    } else {
        //scanner.update();
        player.update(tilemap);
        camera.follow(
        targetX, 
        targetY, 
        0.1f, // Lerp factor
        mapWidthPx, 
        mapHeightPx
        );
    }
}

void PlayState::drawTopScreen() {
    tilemap.draw(camera.getX(), camera.getY());
    player.draw(camera.getX(), camera.getY());

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
