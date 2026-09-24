//src/entities/player/AttackBox.cpp
#include "AttackBox.hpp"
#include <3ds.h>
#include "entities/player/Player.hpp"

AttackBox::AttackBox(Player& player) : player(player) {}

void AttackBox::draw(float cameraX, float cameraY) const
{
    if(!isActive) return;
    C2D_DrawRectSolid(
        xPosition - cameraX,
        yPosition - cameraY,
        0.6f,
        width,
        height,
        C2D_Color32(255, 0, 0, 255)
    );
    printf("AttackBox Drawn at: (%.2f, %.2f)\n", xPosition - cameraX, yPosition);

    
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
    attackDuration = 20;
    
}

void AttackBox::update(){
    if(!isActive) return;

    attackDuration--;
    if(attackDuration <= 0){
        isActive = false;
    }
}