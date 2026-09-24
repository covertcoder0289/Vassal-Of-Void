#include "entities/EntityManager.hpp"
#include <algorithm>

void EntityManager::addEntity(std::unique_ptr<Entity> entity) {
    entities.push_back(std::move(entity));
}

void EntityManager::update() {
    for (auto& entity : entities) {
        if (entity->getIsActive()) {
            entity->update();
        }
    }

    // Erase-remove idiom: entities that died this frame (isActive == false,
    // e.g. from takeDamage() dropping health to 0) get their memory freed
    // right here, not left dangling in the vector.
    entities.erase(
        std::remove_if(
            entities.begin(),
            entities.end(),
            [](const std::unique_ptr<Entity>& e) { return !e->getIsActive(); }
        ),
        entities.end()
    );
}

void EntityManager::draw(float cameraX, float cameraY) const {
    for (const auto& entity : entities) {
        entity->draw(cameraX, cameraY);
    }
}

void EntityManager::clear() {
    entities.clear(); // unique_ptr destructors run here — every enemy's memory freed at once
}

void EntityManager::damageEntitiesInBounds(const AABB& attackBounds, float damage) {
    for (auto& entity : entities) {
        if (!entity->getIsActive()) continue;
        if (aabbOverlap(entity->getBounds(), attackBounds)) {
            entity->takeDamage(damage);
        }
    }
}