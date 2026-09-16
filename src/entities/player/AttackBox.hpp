//src/entities/player/AttackBox.hpp
#pragma once

class AttackBox{
public:
    void update();
    void lightAttack();

private:
    float width;
    float height;
    float damage;
};