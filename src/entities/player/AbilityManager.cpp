//src/entities/player/AbilityManager.cpp
#include "AbilityManager.hpp"
#include "Player.hpp"

void AbilityManager::doubleJump(Player& player){
    //if(player.hasAbility(player.ABILITY_DOUBLE_JUMP) 
    //&& player.getState() != Player::PlayerState::DoubleJumping){
        if(!player.getHasDoubleJumped()){
            player.setVelocityY(-8.5f);
            player.setState(Player::PlayerState::DoubleJumping);
            player.setHasDoubleJumped(true);
        }
    //}
}

//player.hasAbility(player.ABILITY_WALL_JUMP)
void AbilityManager::wallJump(Player& player){
    if(player.getState() == Player::PlayerState::WallSliding){
        player.setState(Player::PlayerState::WallJump);
        player.setHorizontalInputLockCounter(8); // Lock horizontal input for 20 frames
        player.setVelocityY(player.getJumpVelocity());
        player.setVelocityX(-player.getFacingDirection() * 4.5f); // Push player away from wall
    }
}

void AbilityManager::dash(Player& player){
    if(player.getState() != Player::PlayerState::Dashing){
        player.setState(Player::PlayerState::Dashing);
        player.setDashDurationCounter(18); // Dash lasts for 50 frames
        player.setHorizontalInputLockCounter(8); // Lock horizontal input for 8 frames
        player.setVelocityX(player.getFacingDirection() * 10.0f); // Dash speed
        player.setDashCooldownCounter(120); // Set cooldown for 120 frames
    }
}