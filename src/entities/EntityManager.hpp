#pragma once
#include <vector>
#include <memory>
#include "entities/Entity.hpp"
#include "physics/AABB.hpp"

class EntityManager {
public:
    void addEntity(std::unique_ptr<Entity> entity);
    void update();
    void draw(float cameraX, float cameraY) const;

    // Called from PlayState::cleanup() — frees every entity's memory
    // when the room/state is torn down.
    void clear();

    // Applies damage to any active entity whose bounds overlap attackBounds.
    // No dynamic_cast needed — takeDamage() is virtual on Entity itself.
    void damageEntitiesInBounds(const AABB& attackBounds, float damage);

    size_t getActiveCount() const { return entities.size(); }

private:
    std::vector<std::unique_ptr<Entity>> entities;
};