#pragma once
#include "physics/AABB.hpp"

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void update() = 0;
    virtual void draw() const = 0;
    
    AABB getBounds() const { return {xPosition, yPosition, width, height}; }
    virtual void onCollision(Entity& otherEntity) = 0;

    //Setters
    void setPosition(float newX, float newY) { xPosition = newX; yPosition = newY; }
    void setVelocityX(float vx) { velocityX = vx; }
    void setVelocityY(float vy) { velocityY = vy; }
    void setOnGround(bool grounded) { isOnGround = grounded; }

    //Getters
    float getX() const { return xPosition; }
    float getY() const { return yPosition; }
    float getVelocityX() const { return velocityX; }
    float getVelocityY() const { return velocityY; }
    bool getOnGround() const { return isOnGround; }
    bool getIsActive() const { return isActive; }

protected:
    // Core spatial state
    float spawnX = 0.0f;
    float spawnY = 0.0f;
    int spawnRoomId = 0;
    float xPosition = 0.0f;
    float yPosition = 0.0f;
    float width = 0.0f;
    float height = 0.0f; 
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    
    bool isActive = true;
    bool isOnGround = false;
    int facingDirection = 1; // 1 = Right, -1 = Left
};