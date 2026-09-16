//src/entities/player/AbilityManager.cpp
#include "AbilityManager.hpp"
#include "Player.hpp"

void AbilityManager::doubleJump(Player& player){
    //if(player.hasAbility(player.ABILITY_DOUBLE_JUMP) 
    //&& player.getState() != Player::PlayerState::DoubleJumping){
            player.setVelocityY(-8.5f);
            player.setState(Player::PlayerState::DoubleJumping);
    //}
}