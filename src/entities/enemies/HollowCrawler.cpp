#include "HollowCrawler.hpp"
#include "entities/player/Player.hpp"

HollowCrawler::HollowCrawler(float startX, float startY,Player& player)
    : Enemy(/* health = */ 20.0f, /* damage = */ 10.0f),player(player),playerDetection(player,*this)
{
    
    xPosition = startX;
    yPosition = startY;
    width = 32.0f;
    height = 32.0f;
}

void HollowCrawler::update() 
{
    // Crawl logic (patrol ground, check tile edges)
    xPosition += velocityX;
     //printf("Player distance: %f\n", playerDistance);
    //playerDetection.setDetectionArea({getXCenter(),getYCenter()});
    playerDetection.update();
}

// HollowCrawler.cpp
void HollowCrawler::draw(float cameraX, float cameraY) const {
    C2D_DrawRectSolid(
        xPosition - cameraX,
        yPosition - cameraY,
        0.5f,
        width,
        height,
        Colors::white
    );
    //C2D_DrawCircleSolid(xPosition - cameraX, yPosition - cameraY,0.5,150.0f,C2D_Color32(180, 180, 180, 50));

        //detection area visualisation
    playerDetection.draw(cameraX,cameraY);
}

void HollowCrawler::draw() const{
    //     C2D_DrawRectSolid(
    //     xPosition,
    //     yPosition,
    //     0.5f,
    //     32.0f,
    //     32.0f,
    //     Colors::green
    // );

}

void HollowCrawler::onCollision(Entity& otherEntity) {
    // Handle collision against player or attack hitboxes
}