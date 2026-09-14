//src/entities/player/Player.cpp
#include "entities/player/Player.hpp"
#include "core/InputMap.hpp"

#ifndef TOP_SCREEN_WIDTH
    #define TOP_SCREEN_WIDTH 400
    #define TOP_SCREEN_HEIGHT 240
#endif

// Anonymous namespace keeps playerData internal to Player.cpp
namespace {
    struct {
        float width = 16.0f;
        float height = 32.0f;
        float xPosition = (TOP_SCREEN_WIDTH - 16.0f) * 0.5f;
        float yPosition = (TOP_SCREEN_HEIGHT - 16.0f) * 0.5f;
        float moveSpeed = 2.0f;
        float gravity = 0.45f;
        float jumpVelocity = -7.5f;
        float maxFallSpeed = 8.0f;//6
        float jumpCutMultiplier = 0.5f;
        bool isOnGround = false;
        u32 color = Colors::white;
        int facingDirection = 1;
    } playerData;
}

Player::Player() {
    width = playerData.width;
    height = playerData.height;
    xPosition = playerData.xPosition;
    yPosition = playerData.yPosition;
    moveSpeed = playerData.moveSpeed;
    gravity = playerData.gravity;
    jumpVelocity = playerData.jumpVelocity;
    maxFallSpeed = playerData.maxFallSpeed;
    jumpCutMultiplier = playerData.jumpCutMultiplier; // Fixed comma to semicolon
    isOnGround = playerData.isOnGround;
    color = playerData.color;
    facingDirection = playerData.facingDirection;
}

bool Player::isSolidTile(const Tilemap& tilemap, float worldX, float worldY) const {
    int tileType = tilemap.getTileAtPosition(worldX, worldY);
    // Consider both GROUND (1) and PLATFORM (2) as solid collisions
    return tileType == TileType::GROUND || tileType == TileType::PLATFORM;
}

void Player::update(const Tilemap& tilemap) {
    float moveX, moveY;
    InputMap::getMoveAxis(moveX, moveY);
    velocityX = moveX * moveSpeed;
    const float facingThreshold = 0.2f;
    if (moveX > facingThreshold)      facingDirection = 1;
    else if (moveX < -facingThreshold) facingDirection = -1;
    // else: keep last facing — don't reset to a default on release

    // Apply Gravity
    velocityY += gravity;
    if (velocityY > maxFallSpeed) {
        velocityY = maxFallSpeed;
    }

    // -------------------------------------------------------------
    // VERTICAL MOVEMENT & COLLISION RESOLUTION
    // -------------------------------------------------------------
    if (isOnGround && InputMap::isActionDown(Action::Jump)) {
        velocityY = jumpVelocity;
        isOnGround = false;
    }

    if (!isOnGround && velocityY < 0.0f && InputMap::isActionUp(Action::Jump)) {
        velocityY *= jumpCutMultiplier;
    }

    yPosition += velocityY;
    isOnGround = false;

    if (velocityY > 0.0f) { // Moving Down (Falling)
        // Sample bottom-left and bottom-right corners of player box
        float footY = yPosition + height;
        float leftX = xPosition + 1.0f;         // 1px padding to avoid edge clipping
        float rightX = xPosition + width - 1.0f;

        if (isSolidTile(tilemap, leftX, footY) || isSolidTile(tilemap, rightX, footY)) {
            // Find the top edge of the tile row we collided with
            int targetRow = tilemap.worldToRow(footY);
            yPosition = (targetRow * Tilemap::TILE_SIZE) - height;
            velocityY = 0.0f;
            isOnGround = true;
        }
    } 
    else if (velocityY < 0.0f) { // Moving Up (Jumping)
        // Sample top-left and top-right corners
        float headY = yPosition;
        float leftX = xPosition + 1.0f;
        float rightX = xPosition + width - 1.0f;

        if (isSolidTile(tilemap, leftX, headY) || isSolidTile(tilemap, rightX, headY)) {
            // Push player below the tile bottom edge
            int targetRow = tilemap.worldToRow(headY);
            yPosition = (targetRow + 1) * Tilemap::TILE_SIZE;
            velocityY = 0.0f;
        }
    }

    // -------------------------------------------------------------
    // HORIZONTAL MOVEMENT & COLLISION RESOLUTION
    // -------------------------------------------------------------
    xPosition += velocityX;

    if (velocityX > 0.0f) { // Moving Right
        float rightX = xPosition + width;
        float topY = yPosition + 1.0f;
        float bottomY = yPosition + height - 1.0f;

        if (isSolidTile(tilemap, rightX, topY) || isSolidTile(tilemap, rightX, bottomY)) {
            int targetCol = tilemap.worldToCol(rightX);
            xPosition = (targetCol * Tilemap::TILE_SIZE) - width;
            velocityX = 0.0f;
        }
    } 
    else if (velocityX < 0.0f) { // Moving Left
        float leftX = xPosition;
        float topY = yPosition + 1.0f;
        float bottomY = yPosition + height - 1.0f;

        if (isSolidTile(tilemap, leftX, topY) || isSolidTile(tilemap, leftX, bottomY)) {
            int targetCol = tilemap.worldToCol(leftX);
            xPosition = (targetCol + 1) * Tilemap::TILE_SIZE;
            velocityX = 0.0f;
        }
    }
}
void Player::update() {
// Left empty on purpose to satisfy linker. Can be used when updating without tilemap context
}

void Player::draw() const {
    u32 debugColor = isOnGround ? C2D_Color32(0,255,0,255) : color;
    C2D_DrawRectSolid(xPosition, yPosition, 0.5f, width, height, debugColor);
}

void Player::onCollision(Entity& otherEntity) {
    // Collision logic goes here
}