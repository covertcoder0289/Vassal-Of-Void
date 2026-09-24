// src/entities/player/Player.cpp

#include "entities/player/Player.hpp"
#include "core/InputMap.hpp"
#include "entities/player/AbilityManager.hpp"

#ifndef TOP_SCREEN_WIDTH
    #define TOP_SCREEN_WIDTH 400
    #define TOP_SCREEN_HEIGHT 240
#endif

// Anonymous namespace keeps playerData internal to Player.cpp
namespace
{
    struct
    {
        float width = 40.0f;
        float height = 40.0f;

        float xPosition = (TOP_SCREEN_WIDTH - 40.0f) * 0.5f;
        float yPosition = (TOP_SCREEN_HEIGHT - 40.0f) * 0.5f;

        float moveSpeed = 2.0f;
        float runSpeed = 4.0f;

        float gravity = 0.43f;
        float jumpVelocity = -8.8f;
        float maxFallSpeed = 4.0f;
        float jumpCutMultiplier = 0.5f;

        bool isOnGround = false;

        u32 color = Colors::white;

        int facingDirection = 1;

    } playerData;
}


// =============================================================
// CONSTRUCTOR
// =============================================================

Player::Player() : attackBox(*this)
{
    width = playerData.width;
    height = playerData.height;

    xPosition = playerData.xPosition;
    yPosition = playerData.yPosition;

    moveSpeed = playerData.moveSpeed;
    runSpeed = playerData.runSpeed;

    gravity = playerData.gravity;
    jumpVelocity = playerData.jumpVelocity;
    maxFallSpeed = playerData.maxFallSpeed;
    jumpCutMultiplier = playerData.jumpCutMultiplier;

    isOnGround = playerData.isOnGround;

    color = playerData.color;

    facingDirection = playerData.facingDirection;
}


// =============================================================
// TILE COLLISION
// =============================================================

bool Player::isSolidTile(
    const Tilemap& tilemap,
    float worldX,
    float worldY
) const
{
    int tileType = tilemap.getTileAtPosition(worldX, worldY);

    return tileType == TileType::GROUND ||
           tileType == TileType::PLATFORM;
}


// =============================================================
// UPDATE
// =============================================================

void Player::update(const Tilemap& tilemap)
{
    // ---------------------------------------------------------
    // INPUT
    // ---------------------------------------------------------

    float moveX;
    float moveY;

    InputMap::getMoveAxis(moveX, moveY);

    const float stickDeadZone = 0.2f;


    // ---------------------------------------------------------
    // UPDATE FACING DIRECTION
    // ---------------------------------------------------------

    if (moveX > stickDeadZone)
    {
        facingDirection = 1;
    }
    else if (moveX < -stickDeadZone)
    {
        facingDirection = -1;
    }


    // ---------------------------------------------------------
    // ATTACK
    // ---------------------------------------------------------

    if (InputMap::isActionDown(Action::Attack))
    {
        currentState = PlayerState::Attacking;

        attackBox.lightAttack();
    }

    if(attackBox.getIsActive())
        currentState = PlayerState::Attacking;

    // ---------------------------------------------------------
    // DASH INPUT
    // ---------------------------------------------------------

    if (InputMap::isActionDown(Action::Dash))
    {
        if (dashCooldownCounter == 0)
        {
            abilityManager.dash(*this);
        }
    }


    // ---------------------------------------------------------
    // DASH COOLDOWN
    // ---------------------------------------------------------

    if (dashCooldownCounter > 0)
    {
        dashCooldownCounter--;
    }


    // =========================================================
    // DASHING
    // =========================================================
    //
    // Dashing is handled separately from normal movement.
    //
    // This is important because the normal movement code below
    // can otherwise change:
    //
    //      Dashing -> Falling
    //      Dashing -> Jumping
    //      Dashing -> WallSliding
    //
    // before the dash animation gets a chance to play.
    //
    // =========================================================

    if (currentState == PlayerState::Dashing)
    {
        // Update dash animation
        dashAnimation.update();


        // Count down dash duration
        if (dashDurationCounter > 0)
        {
            dashDurationCounter--;
        }


        // -----------------------------------------------------
        // Move horizontally during dash
        // -----------------------------------------------------

        xPosition += velocityX;


        // -----------------------------------------------------
        // Dash collision - moving right
        // -----------------------------------------------------

        if (velocityX > 0.0f)
        {
            float rightX = xPosition + width;

            float topY = yPosition + 1.0f;
            float bottomY = yPosition + height - 1.0f;

            if (
                isSolidTile(tilemap, rightX, topY) ||
                isSolidTile(tilemap, rightX, bottomY)
            )
            {
                int targetCol = tilemap.worldToCol(rightX);

                xPosition =
                    (targetCol * Tilemap::TILE_SIZE) - width;

                velocityX = 0.0f;

                // Stop the dash when hitting a wall
                dashDurationCounter = 0;
            }
        }


        // -----------------------------------------------------
        // Dash collision - moving left
        // -----------------------------------------------------

        else if (velocityX < 0.0f)
        {
            float leftX = xPosition;

            float topY = yPosition + 1.0f;
            float bottomY = yPosition + height - 1.0f;

            if (
                isSolidTile(tilemap, leftX, topY) ||
                isSolidTile(tilemap, leftX, bottomY)
            )
            {
                int targetCol = tilemap.worldToCol(leftX);

                xPosition =
                    (targetCol + 1) * Tilemap::TILE_SIZE;

                velocityX = 0.0f;

                // Stop the dash when hitting a wall
                dashDurationCounter = 0;
            }
        }


        // -----------------------------------------------------
        // End dash
        // -----------------------------------------------------

        if (dashDurationCounter == 0)
        {
            currentState = PlayerState::Falling;

            velocityX = 0.0f;
        }


        // Attack box still needs updating
        attackBox.update();


        // IMPORTANT:
        //
        // Don't run normal gravity/jump/falling code this frame.
        //
        return;
    }


    // =========================================================
    // NOT DASHING
    // =========================================================

    // Dash animation should return to frame 0 when we're not
    // dashing.
    dashAnimation.reset();


    // ---------------------------------------------------------
    // NORMAL HORIZONTAL MOVEMENT
    // ---------------------------------------------------------

    if (horizontalInputLockCounter == 0)
    {
        if (
            (moveX < -stickDeadZone && moveX >= -1.0f) ||
            (moveX > stickDeadZone && moveX <= 1.0f)
        )
        {
            if (isOnGround)
            {
                currentState = PlayerState::Walking;
            }

            velocityX = moveX * moveSpeed;
        }
        else
        {
            if (isOnGround && currentState != PlayerState::Attacking)
            {
                currentState = PlayerState::Idle;
            }

            velocityX = 0.0f;
        }
    }


    // ---------------------------------------------------------
    // IDLE ANIMATION
    // ---------------------------------------------------------

    if (currentState == PlayerState::Idle)
    {
        idleAnimation.update();
    }
    else
    {
        idleAnimation.reset();
    }


    // ---------------------------------------------------------
    // HORIZONTAL INPUT LOCK
    // ---------------------------------------------------------

    if (horizontalInputLockCounter > 0)
    {
        horizontalInputLockCounter--;
    }


    // =========================================================
    // GRAVITY
    // =========================================================

    velocityY += gravity;

    if (velocityY > maxFallSpeed)
    {
        velocityY = maxFallSpeed;
    }


    // =========================================================
    // JUMP INPUT
    // =========================================================

    if (InputMap::isActionDown(Action::Jump))
    {
        if (isOnGround)
        {
            velocityY = jumpVelocity;

            isOnGround = false;

            hasDoubleJumped = false;

            currentState = PlayerState::Jumping;
        }
        else if (
            currentState == PlayerState::Jumping ||
            currentState == PlayerState::Falling
        )
        {
            abilityManager.doubleJump(*this);
        }
        else if (currentState == PlayerState::WallSliding)
        {
            abilityManager.wallJump(*this);
        }
    }


    // =========================================================
    // VARIABLE JUMP HEIGHT
    // =========================================================

    if (
        !isOnGround &&
        velocityY < 0.0f &&
        InputMap::isActionUp(Action::Jump)
    )
    {
        velocityY *= jumpCutMultiplier;
    }


    // =========================================================
    // VERTICAL MOVEMENT
    // =========================================================

    yPosition += velocityY;

    isOnGround = false;


    // ---------------------------------------------------------
    // FALLING
    // ---------------------------------------------------------

    if (velocityY > 0.0f)
    {
        currentState = PlayerState::Falling;

        float footY = yPosition + height;

        float leftX = xPosition + 1.0f;
        float rightX = xPosition + width - 1.0f;

        if (
            isSolidTile(tilemap, leftX, footY) ||
            isSolidTile(tilemap, rightX, footY)
        )
        {
            int targetRow = tilemap.worldToRow(footY);

            yPosition =
                (targetRow * Tilemap::TILE_SIZE) - height;

            currentState = PlayerState::Idle;

            velocityY = 0.0f;

            isOnGround = true;

            hasDoubleJumped = false;
        }
    }


    // ---------------------------------------------------------
    // JUMPING
    // ---------------------------------------------------------

    else if (velocityY < 0.0f)
    {
        currentState = PlayerState::Jumping;

        float headY = yPosition;

        float leftX = xPosition + 1.0f;
        float rightX = xPosition + width - 1.0f;

        if (
            isSolidTile(tilemap, leftX, headY) ||
            isSolidTile(tilemap, rightX, headY)
        )
        {
            int targetRow = tilemap.worldToRow(headY);

            yPosition =
                (targetRow + 1) * Tilemap::TILE_SIZE;

            velocityY = 0.0f;
        }
    }


    // =========================================================
    // HORIZONTAL MOVEMENT
    // =========================================================

    xPosition += velocityX;


    // ---------------------------------------------------------
    // MOVING RIGHT
    // ---------------------------------------------------------

    if (velocityX > 0.0f)
    {
        float rightX = xPosition + width;

        float topY = yPosition + 1.0f;
        float bottomY = yPosition + height - 1.0f;

        if (
            isSolidTile(tilemap, rightX, topY) ||
            isSolidTile(tilemap, rightX, bottomY)
        )
        {
            int targetCol = tilemap.worldToCol(rightX);

            xPosition =
                (targetCol * Tilemap::TILE_SIZE) - width;

            velocityX = 0.0f;

            if (!isOnGround)
            {
                currentState = PlayerState::WallSliding;

                velocityY *= 0.1f;
            }
        }
    }


    // ---------------------------------------------------------
    // MOVING LEFT
    // ---------------------------------------------------------

    else if (velocityX < 0.0f)
    {
        float leftX = xPosition;

        float topY = yPosition + 1.0f;
        float bottomY = yPosition + height - 1.0f;

        if (
            isSolidTile(tilemap, leftX, topY) ||
            isSolidTile(tilemap, leftX, bottomY)
        )
        {
            int targetCol = tilemap.worldToCol(leftX);

            xPosition =
                (targetCol + 1) * Tilemap::TILE_SIZE;

            velocityX = 0.0f;

            if (!isOnGround)
            {
                currentState = PlayerState::WallSliding;

                velocityY *= 0.1f;
            }
        }
    }


    // =========================================================
    // ATTACK BOX
    // =========================================================

    attackBox.update();
}


// =============================================================
// UPDATE WITHOUT TILEMAP
// =============================================================

void Player::update()
{
    // Left empty on purpose to satisfy linker.
    // Can be used when updating without tilemap context.
}


// =============================================================
// DRAW
// =============================================================

void Player::draw(float cameraX, float cameraY) const
{
    float screenX = xPosition - cameraX;
    float screenY = yPosition - cameraY;

        attackBox.draw(
        cameraX,
        cameraY
    );

    C2D_DrawParams params =
    {
        .pos =
        {
            screenX,
            screenY,

            (facingDirection > 0)
                ? width
                : -width,

            height
        },

        .center =
        {
            0.0f,
            0.0f
        },

        .depth = 0.5f,

        .angle = 0.0f
    };


    C2D_Image spriteToDraw;


    // ---------------------------------------------------------
    // IDLE
    // ---------------------------------------------------------

    if (currentState == PlayerState::Idle)
    {
        spriteToDraw =
            idleAnimation.getCurrentFrame();
    }


    // ---------------------------------------------------------
    // DASH
    // ---------------------------------------------------------

     else if (currentState == PlayerState::Dashing)
    {
        spriteToDraw =
            dashAnimation.getCurrentFrame();
    }


    // ---------------------------------------------------------
    // DEFAULT
    // ---------------------------------------------------------

    else
    {
        spriteToDraw = playerSprite;
    }


    C2D_DrawImage(
        spriteToDraw,
        &params
    );



}


// =============================================================
// COLLISION
// =============================================================

void Player::onCollision(Entity& otherEntity)
{
    // Collision logic goes here
}


// =============================================================
// IDLE ANIMATION
// =============================================================

void Player::setIdleAnimation(
    C2D_Image spriteSheet,
    int frameCount
)
{
    idleAnimation.setSpriteSheet(
        spriteSheet,
        32,
        32,
        frameCount
    );
}


// =============================================================
// DASH ANIMATION
// =============================================================

void Player::setDashAnimation(
    C2D_Image spriteSheet,
    int frameCount
)
{
    dashAnimation.setSpriteSheet(
        spriteSheet,
        32,
        32,
        frameCount
    );
    dashAnimation.setFrameDuration(3); // Set the frame duration for the dash animation
}