#include "StateManager.hpp"

void StateManager::setState(std::unique_ptr<GameState> newState) {
    pending = std::move(newState);
}

void StateManager::update() {
    // 1. Process pending state swap
    if (pending) {
        // Unload resources of previous state if applicable
        if (current) {
            current->cleanup();
        }

        // Swap state
        current = std::move(pending);

        // Initialize new state (loads RomFS textures, spritesheets, etc.)
        if (current) {
            bool success = current->init();
            if (!success) {
                // TEMP: make failure impossible to miss
            }
        }
    }

    // 2. Run active state update loop
    if (current) {
        current->update();
    }
}

void StateManager::drawTopScreen() {
    if (current) {
        current->drawTopScreen();
    }
}

void StateManager::drawBottomScreen() {
    if (current) {
        current->drawBottomScreen();
    }
}