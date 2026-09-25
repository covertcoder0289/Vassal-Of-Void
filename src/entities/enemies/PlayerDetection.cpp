//src/entities/enemies/PlayerDetection.cpp
#include "PlayerDetection.hpp"
#include "entities/player/Player.hpp"
#include "entities/enemies/Enemy.hpp"

PlayerDetection::PlayerDetection(Player& player, Entity& enemy):player(player)
,enemy(enemy)
{
}

void PlayerDetection::update()
{
    xPosition = enemy.getXCenter();
    yPosition = enemy.getYCenter();

    if(playerInRange()){
        targetPlayer();
    }else{
        enemy.setVelocityX(0);
    }
}

bool PlayerDetection::playerInRange()
{
    float dx = player.getXCenter() - enemy.getXCenter();
    float dy = player.getYCenter() - enemy.getYCenter();

    // Compare squared distance against squared radius
    float distanceSquared = (dx * dx) + (dy * dy);
    float radiusSquared = detectionRadius * detectionRadius;
    
    return distanceSquared <= radiusSquared;
}

void PlayerDetection::draw(float cameraX, float cameraY) const{
    float screenX = xPosition - cameraX;
    float screenY = yPosition - cameraY;
    C2D_DrawCircleSolid(screenX,screenY,0.5,detectionRadius,C2D_Color32(180,180,180,50));
    
}

void PlayerDetection::draw() const{

}

void PlayerDetection::onCollision(Entity& entity){

}

void PlayerDetection::targetPlayer()
{
    playerDistance = player.getX() - xPosition;
    if(playerDistance < 0){
        enemy.setVelocityX(-0.85f);
    }else if(playerDistance > 0){
        enemy.setVelocityX(0.85f);
    }
}
