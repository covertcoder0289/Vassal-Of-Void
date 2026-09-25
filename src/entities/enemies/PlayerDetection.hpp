//src/entities/enemies/PlayerDetection.hpp
#pragma once

#include "entities/Entity.hpp"

class Enemy;
class Player;
class PlayerDetection : public Entity{
    struct Point
    {
        float x = 0.0f;
        float y = 0.0f;    
    };

    struct Circle
    {
        float centerX = 0.0f;
        float centerY = 0.0f;
        float radius = 0.0f;
    };


public:
    PlayerDetection(Player& player,Entity& enemy);
    bool playerInRange();
    void draw(float cameraX, float cameraY) const override;
    void draw() const override;
    void update() override;
    void onCollision(Entity& entity) override;
    void setDetectionArea(Circle newDetectionArea){detectionArea = newDetectionArea;};
    void targetPlayer();

private:
    Player& player;
    Entity& enemy;
    Circle detectionArea = {};
    float detectionRadius = 200.0f;
    float playerDistance = 0.0f;

};