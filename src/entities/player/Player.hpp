//src/entities/Player.hpp
#pragma once

#include <3ds.h>
#include <citro2d.h>
#include "common/colors.hpp"
#include "entities/Entity.hpp"
#include "world/Tilemap.hpp"
#include "AbilityManager.hpp"
#include "AttackBox.hpp"
#include "graphics/Animation.hpp"


class Player : public Entity {
public:
    enum class PlayerState{
        Idle,
        Walking,
        Running,
        Crouching,
        Jumping,
        DoubleJumping,
        Falling,
        Landing,
        Attacking,
        Hurt,
        Knockback,
        Dashing,
        WallContact,
        WallSliding,
        WallJump,
        Dead,
        Healing,
        ChargingVoid,
        VoidOverload,
        Interacting,
        TransitioningRoom
    };

    enum AbilityFlags : uint32_t {
        ABILITY_NONE        = 0,
        ABILITY_DASH        = 1 << 0, // 0001
        ABILITY_DOUBLE_JUMP = 1 << 1, // 0010
        ABILITY_WALL_JUMP   = 1 << 2, // 0100
        ABILITY_CHARGING_VOID= 1 << 3  // 1000
    };

    Player();
    ~Player() override = default;

    void update();
    void update(const Tilemap& tilemap);
    void draw(float cameraX, float cameraY) const;
    void draw() const override { draw(0.0f, 0.0f); }
    void onCollision(Entity& otherEntity) override;

    // Helper method to check if a specific world point is solid
    bool isSolidTile(const Tilemap& tilemap, float worldX, float worldY) const;

    PlayerState getState() { return currentState; }
    float getXVelocity() const { return velocityX; }
    float getYVelocity() const { return velocityY;}
    int getFacingDirection() const { return facingDirection; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    float getJumpVelocity() const { return jumpVelocity; }
    bool getHasDoubleJumped() const { return hasDoubleJumped; }
    
    void setState(PlayerState newState) { currentState = newState; }
    void setXVelocity(float newVx) { velocityX = newVx; }
    void setYVelocity(float newVy) { velocityY = newVy; }
    void setHorizontalInputLockCounter(int frames) { horizontalInputLockCounter = frames; }
    void setHasDoubleJumped(bool value) { hasDoubleJumped = value; }
    void setDashCooldownCounter(int frames) { dashCooldownCounter = frames; }
    void setDashDurationCounter(int frames) { dashDurationCounter = frames; }
    void setSprite(C2D_Image newSprite) { playerSprite = newSprite; }
    void setIdleAnimation(C2D_Image spriteSheet, int frameCount);
    void setWalkAnimation(C2D_Image spriteSheet, int frameCount);
    void setDashAnimation(C2D_Image spriteSheet, int frameCount);
    void setSwordAnimation(C2D_Image spriteSheet, int frameCount) { attackBox.setSwordAnimation(spriteSheet, frameCount); }
    // Helper methods for common impulse operations
    void setVelocity(float newVx, float newVy) { velocityX = newVx; velocityY = newVy; }
    void addYVelocity(float amount) { velocityY += amount; }
    void addXVelocity(float amount) { velocityX += amount; }

    //ability methods
    void unlockAbility(AbilityFlags ability) {
        unlockedAbilities |= ability;
    }

    void lockAbility(AbilityFlags ability) {
        unlockedAbilities &= ~ability;
    }

    bool hasAbility(AbilityFlags ability) const {
        return (unlockedAbilities & ability) != 0;
    }

private:
    u32 color;
    float moveSpeed = 0.0f;
    float runSpeed = 0.0f;
    float gravity = 0.0f;
    float jumpVelocity = 0.0f;
    float maxFallSpeed = 0.0f;
    float jumpCutMultiplier = 0.0f;
    PlayerState currentState = PlayerState::Idle;
    uint32_t unlockedAbilities = ABILITY_NONE;
    AbilityManager abilityManager;
    AttackBox attackBox;
    bool hasDoubleJumped = false;
    int horizontalInputLockCounter = 0; // Counter to lock horizontal input for a few frames after wall jump
    int dashCooldownCounter = 0; // Counter to manage dash cooldown
    C2D_Image playerSprite;
    Animation idleAnimation;
    Animation walkAnimation;
    Animation dashAnimation;
    Animation attackAnimation;
    int dashDurationCounter = 0; // Counter to manage dash duration
};