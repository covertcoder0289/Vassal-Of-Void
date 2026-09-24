//src/state/PlayState.cpp
#include "PlayState.hpp"
#include "MenuState.hpp"
#include "StateManager.hpp"
//#include "../physics/CollisionWorld.hpp"
#include <3ds.h>
#include "sprites.h"
#include "world/RoomLoader.hpp"
#include "core/InputMap.hpp"

//used for camera
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

//used for looking up/down
#define Y_CAMERA_OFFSET 90.0

PlayState::PlayState(StateManager& stateManager) 
    : stateManager(stateManager),camera(SCREEN_WIDTH, SCREEN_HEIGHT) {
    buildTestRoom();

    pauseOverlay = std::make_unique<PauseState>(stateManager, [this]() { isPaused = false; });
}

PlayState::~PlayState()
{  
    cleanup();
}

bool PlayState::init() {
    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    
    if (!spriteSheet) 
    {
        svcBreak(USERBREAK_PANIC);
        return false; // Failed to load texture sheet
    }

    blockTileImage = C2D_SpriteSheetGetImage(spriteSheet, sprites_test_room_block_idx);
    // Pass the tile image handle to the tilemap so it knows what to render
    tilemap.setTileTexture(blockTileImage);
    C2D_Image idleSheet = C2D_SpriteSheetGetImage(spriteSheet, sprites_idle_spritesheet_idx);
    C2D_Image voidSurgeSheet = C2D_SpriteSheetGetImage(spriteSheet, sprites_void_surge_idx);
    C2D_Image attackSheet = C2D_SpriteSheetGetImage(spriteSheet, sprites_attack_sword_idx);
    C2D_Image walkSheet = C2D_SpriteSheetGetImage(spriteSheet,sprites_walking_spritesheet_idx);

    player.setIdleAnimation(idleSheet, 9);
    player.setWalkAnimation(walkSheet, 9);
    player.setDashAnimation(voidSurgeSheet, 18);
    player.setSwordAnimation(attackSheet, 4);

    C2D_Image playerSprite = C2D_SpriteSheetGetImage(spriteSheet, sprites_vassal_v1_idx);
    player.setSprite(playerSprite);
    return true;
}

void PlayState::cleanup() 
{
    // Free memory taken up by the sprite sheet
    if (spriteSheet) {
        C2D_SpriteSheetFree(spriteSheet);
        spriteSheet = nullptr;
    }
    entityManager.clear();
}

void PlayState::buildTestRoom() 
{
    int cols, rows;
    std::vector<int> roomData;
    if (!RoomLoader::loadFromFile("romfs:/rooms/enemies.json", cols, rows, roomData))
        svcBreak(USERBREAK_PANIC); // fail loud, same instinct as the sprite-loading bug
    
    tilemap.loadMap(cols, rows, roomData);
    float spawnX, spawnY;
    if (RoomLoader::loadSpawnPoint("romfs:/rooms/enemies.json", spawnX, spawnY)) 
        player.setPosition(spawnX, spawnY);

        entityManager.addEntity(std::make_unique<HollowCrawler>(850.0f, 787.0f));

}


void PlayState::update() 
{
    u32 kDown = hidKeysDown();
    u32 kHeld = hidKeysHeld();

    float targetX = player.getX() + 8.0f;
    float targetY = player.getY() + 8.0f;
    float mapWidthPx = tilemap.getColumns() * 16.0f;
    float mapHeightPx = tilemap.getRows() * 16.0f;

    //look up and down if player is on the ground
    if(kHeld & KEY_UP && player.getOnGround()) 
        targetY -= Y_CAMERA_OFFSET;
    if(kHeld & KEY_DOWN && player.getOnGround()) 
        targetY += Y_CAMERA_OFFSET; 

    if (kDown & KEY_START)
        isPaused = !isPaused;

    if (isPaused) {
        pauseOverlay->update();
    } else {
        //scanner.update();
        player.update(tilemap);
        entityManager.update();
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
    entityManager.draw(camera.getX(), camera.getY());
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
