#pragma once
#include "entities/Entity.hpp"

class Enemy : public Entity {
public:
    enum class EnemyState {
        Idle,
        Walking,
        Attacking,
        Hurt
    };

    Enemy(float hp, float atkDamage) 
        : health(hp), maxHealth(hp), damage(atkDamage), state(EnemyState::Idle) {}

    virtual ~Enemy() override = default;

    // Optional: Provide a default collision handler for all enemies
    void onCollision(Entity& otherEntity) override {
        // Base enemy collision logic (or leave empty for derived classes to override)
    }

    virtual void takeDamage(float amount) {
        health -= amount;
        if (health <= 0.0f) {
            health = 0.0f;
            isActive = false;
        }
    }

protected:
    float health = 10.0f;
    float maxHealth = 10.0f;
    float damage = 5.0f;
    EnemyState state = EnemyState::Idle;
};