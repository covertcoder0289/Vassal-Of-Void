//src/entities/player/AttackBox.cpp
#include "AttackBox.hpp"
#include <3ds.h>
#include "entities/player/Player.hpp"
#include "sprites.h"


AttackBox::AttackBox(Player& player) : player(player) {
}

void AttackBox::draw(float cameraX, float cameraY) const
{
    if (!isActive) return;
    float screenX = xPosition - cameraX;
    float screenY = yPosition - cameraY;
    C2D_DrawParams params =
    {
        .pos =
        {
            screenX,
            screenY,
            (player.getFacingDirection() > 0) ? width - 11.0f : -width + 11.0f, // Adjust the width based on facing direction
            height
        },
        .center ={0.0f, 0.0f},

        .depth = 0.6f,

        .angle = 0.0f
    };
    //if(player.getState() == Player::PlayerState::Attacking)
    C2D_Image spriteToDraw = swordAnimation.getCurrentFrame();
    C2D_DrawImage(spriteToDraw, &params);
    
}

void AttackBox::lightAttack(){

    height = 40.0f;
    width = 40.0f;
    damage = 10.0f;
    
    if(player.getFacingDirection() > 0){
        xPosition = player.getX() + player.getWidth();
    }else{
        xPosition = player.getX() - width;
    }

    yPosition = player.getY();
    isActive = true;
    attackDuration = 16;
    swordAnimation.reset();
    
}

void AttackBox::update(){
    if(!isActive) return;

    swordAnimation.update();

    attackDuration--;
    if(attackDuration <= 0){
        isActive = false;
    }
}

void AttackBox::setSwordAnimation(
    C2D_Image spriteSheet,
    int frameCount
)
{
    swordAnimation.setSpriteSheet(
        spriteSheet,
        40,
        40,
        frameCount
    );
    swordAnimation.setFrameDuration(4); // Set the frame duration for the sword animation
}