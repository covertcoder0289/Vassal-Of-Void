//src/entities/Player.hpp
#pragma once

#include <3ds.h>
#include <citro2d.h>
#include "common/colors.hpp"
#include "entities/Entity.hpp"
#include "world/Tilemap.hpp"
#include "AbilityManager.hpp"


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
        WallSlide,
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
    
    void setState(PlayerState newState) { currentState = newState; }
    void setXVelocity(float newVx) { velocityX = newVx; }
    void setYVelocity(float newVy) { velocityY = newVy; }

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
    bool hasDoubleJumped = false;
};