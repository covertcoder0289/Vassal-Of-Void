//src/entities/Player.hpp
#pragma once

#include <3ds.h>
#include <citro2d.h>
#include "common/colors.hpp"
#include "entities/Entity.hpp"
#include "world/Tilemap.hpp"

class Player : public Entity {
public:
    enum class PlaterState{
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

    Player();
    ~Player() override = default;

    void update();
    void update(const Tilemap& tilemap);
    void draw() const override;
    void onCollision(Entity& otherEntity) override;

    // Helper method to check if a specific world point is solid
    bool isSolidTile(const Tilemap& tilemap, float worldX, float worldY) const;

private:
    u32 color;
    float moveSpeed = 0.0f;
    float gravity = 0.0f;
    float jumpVelocity = 0.0f;
    float maxFallSpeed = 0.0f;
    float jumpCutMultiplier = 0.0f;
    PlaterState currentState = PlaterState::Idle;
};