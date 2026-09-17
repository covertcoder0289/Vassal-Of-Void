#pragma once

class Player;

class AbilityManager{
public:
    AbilityManager() = default;
    ~AbilityManager() = default;
    void doubleJump(Player& player);
    void wallJump(Player& player);
    void dash(Player& player);

};