//src/entities/player/AttackBox.hpp
#pragma once

#include "graphics/Animation.hpp"

class Player;

class AttackBox{
public:
    AttackBox(Player& player);
    void update();
    void lightAttack();
    void draw(float cameraX, float cameraY) const;
    bool getIsActive() const { return isActive; }

    void setSwordAnimation(
    C2D_Image spriteSheet,
    int frameCount
);

private:
    float xPosition = 0.0f;
    float yPosition = 0.0f;
    float xVelocity = 0.0f;
    float yVelocity = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    float damage = 0.0f;
    Player& player;
    bool isActive = false;
    int attackDuration = 0;

    Animation swordAnimation;

};