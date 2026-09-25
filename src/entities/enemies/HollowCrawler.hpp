#pragma once
#include "Enemy.hpp"
#include <3ds.h>
#include <citro2d.h>
#include "common/colors.hpp"
#include "PlayerDetection.hpp"

class HollowCrawler : public Enemy {
public:
    HollowCrawler(float startX, float startY,Player& player);
    ~HollowCrawler() override = default;

    // Must match const-correctness and pure virtual requirements
    void update() override;
    void draw(float cameraX, float cameraY) const override;
    void draw() const override;
    void onCollision(Entity& otherEntity) override;

private:
    float walkSpeed = 0.5f;
    float playerDetectionRange = 50.0f;
    //PlayerDetection playerDetection;
    Player& player;
    PlayerDetection playerDetection;
};