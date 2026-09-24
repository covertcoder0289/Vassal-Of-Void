#include "HollowCrawler.hpp"

HollowCrawler::HollowCrawler(float startX, float startY)
    : Enemy(/* health = */ 20.0f, /* damage = */ 10.0f) 
{
    xPosition = startX;
    yPosition = startY;
    width = 32.0f;
    height = 32.0f;
}

void HollowCrawler::update() {
    // Crawl logic (patrol ground, check tile edges)
    xPosition += velocityX;
}

// HollowCrawler.cpp
void HollowCrawler::draw(float cameraX, float cameraY) const {
    C2D_DrawRectSolid(
        xPosition - cameraX,
        yPosition - cameraY,
        0.5f,
        32.0f,
        32.0f,
        Colors::green
    );
}

void HollowCrawler::draw() const{
        C2D_DrawRectSolid(
        xPosition,
        yPosition,
        0.5f,
        32.0f,
        32.0f,
        Colors::green
    );
}

void HollowCrawler::onCollision(Entity& otherEntity) {
    // Handle collision against player or attack hitboxes
}