//src/entities/player/Player.cpp
#include "entities/player/Player.hpp"
#include "core/InputMap.hpp"
#include "entities/player/AbilityManager.hpp"

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
        float runSpeed = 4.0f;
        float gravity = 0.45f;
        float jumpVelocity = -8.5f;
        float maxFallSpeed = 4.0f;//6
        float jumpCutMultiplier = 0.5f;
        bool isOnGround = false;
        u32 color = Colors::white;
        int facingDirection = 1;
    } playerData;
}

Player::Player() : attackBox(*this) {
    width = playerData.width;
    height = playerData.height;
    xPosition = playerData.xPosition;
    yPosition = playerData.yPosition;
    moveSpeed = playerData.moveSpeed;
    runSpeed = playerData.runSpeed;
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
        // Attack Trigger
    if (InputMap::isActionDown(Action::Attack)) 
    {
        currentState = PlayerState::Attacking;
        attackBox.lightAttack();   
    }
    float moveX, moveY;

    if(InputMap::isActionDown(Action::Dash))
    {
        if(dashCooldownCounter == 0)
            abilityManager.dash(*this);
    }
    if(dashCooldownCounter > 0)
        dashCooldownCounter--;
        
    InputMap::getMoveAxis(moveX, moveY);

    const float stickDeadZone = 0.2f;

    if(horizontalInputLockCounter == 0)
    {
        if((moveX < -stickDeadZone && moveX >= -1.0f) || (moveX > stickDeadZone && moveX <= 1.0f)) 
        {
            if(isOnGround) currentState = PlayerState::Walking;  

            velocityX = moveX * moveSpeed;
        }else
            {
               if(isOnGround) currentState = PlayerState::Idle;
                velocityX = 0.0f;
            }
    }
    // else if(moveX <= -0.9f || moveX >= 0.9f) {
    //     if(isOnGround) currentState = PlayerState::Running;
    //     velocityX = moveX * runSpeed;
    // }

    if (moveX > stickDeadZone) facingDirection = 1;
    else if (moveX < -stickDeadZone) facingDirection = -1;
    // else: keep last facing — don't reset to a default on release

    // Apply Gravity
    velocityY += gravity;
    if (velocityY > maxFallSpeed) {
        velocityY = maxFallSpeed;
    }

    // -------------------------------------------------------------
    // VERTICAL MOVEMENT & COLLISION RESOLUTION
    // -------------------------------------------------------------
    if (InputMap::isActionDown(Action::Jump)) {

        if(isOnGround){
            velocityY = jumpVelocity;
            isOnGround = false;
            hasDoubleJumped = false;
            currentState = PlayerState::Jumping;
            //printf("Player is jumping with velocity of %f\n",velocityY);
        }
        else if((currentState == PlayerState::Jumping || currentState == PlayerState::Falling)){
            abilityManager.doubleJump(*this);
        }else if(currentState == PlayerState::WallSliding){
            abilityManager.wallJump(*this);
        }

    }
    if(horizontalInputLockCounter > 0)
        horizontalInputLockCounter--;



    if (!isOnGround && velocityY < 0.0f && InputMap::isActionUp(Action::Jump)) {
        velocityY *= jumpCutMultiplier;
    }

    yPosition += velocityY;
    isOnGround = false;

    if (velocityY > 0.0f) { // Moving Down (Falling)
        // Sample bottom-left and bottom-right corners of player box
        currentState = PlayerState::Falling;
        float footY = yPosition + height;
        float leftX = xPosition + 1.0f;         // 1px padding to avoid edge clipping
        float rightX = xPosition + width - 1.0f;
        
        if (isSolidTile(tilemap, leftX, footY) || isSolidTile(tilemap, rightX, footY)) {
            // Find the top edge of the tile row we collided with
            int targetRow = tilemap.worldToRow(footY);
            yPosition = (targetRow * Tilemap::TILE_SIZE) - height;
            currentState = PlayerState::Idle;
            velocityY = 0.0f;
            isOnGround = true;
            hasDoubleJumped = false;
        }
    } 
    else if (velocityY < 0.0f) { // Moving Up (Jumping)
        // Sample top-left and top-right corners
        currentState = PlayerState::Jumping;
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

            if(!isOnGround){
                currentState = PlayerState::WallSliding;
                velocityY *= 0.1f;
            }
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

            if(!isOnGround){
                currentState = PlayerState::WallSliding;
                velocityY *= 0.1f;
            }
        }
    }

attackBox.update();
}
void Player::update() {
// Left empty on purpose to satisfy linker. Can be used when updating without tilemap context
}

void Player::draw(float cameraX, float cameraY) const {
    float screenX = xPosition - cameraX;
    float screenY = yPosition - cameraY;
    //u32 debugColor = isOnGround ? C2D_Color32(0,255,0,255) : color;
    C2D_DrawRectSolid(screenX, screenY, 0.5f, width, height, color);

    float playerCenterX = screenX + width / 2;
    float playerCenterY = screenY + height / 2;
    if(facingDirection == 1){
        C2D_DrawTriangle(
            playerCenterX + (width/2),  playerCenterY, C2D_Color32(255, 0, 0, 255),   // Top vertex (Red)
            playerCenterX,  playerCenterY + 5.0f, C2D_Color32(0, 255, 0, 255),   // Bottom-left vertex (Green)
            playerCenterX,  playerCenterY - 5.0f, C2D_Color32(0, 0, 255, 255),   // Bottom-right vertex (Blue)
            0.5f                                           // Depth
        );
    }else{
        C2D_DrawTriangle(
            playerCenterX - (width/2),  playerCenterY, C2D_Color32(255, 0, 0, 255),   // Top vertex (Red)
            playerCenterX,  playerCenterY + 5.0f, C2D_Color32(0, 255, 0, 255),   // Bottom-left vertex (Green)
            playerCenterX,  playerCenterY - 5.0f, C2D_Color32(0, 0, 255, 255),   // Bottom-right vertex (Blue)
            0.5f                                           // Depth
        );
        
    }


        attackBox.draw(cameraX, cameraY);
    
}

void Player::onCollision(Entity& otherEntity) {
    // Collision logic goes here
}