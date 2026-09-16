// Camera.cpp
#include "Camera.hpp"
#include <algorithm>
#include "core/InputMap.hpp"

Camera::Camera(float viewportWidth, float viewportHeight)
    : viewportWidth(viewportWidth), viewportHeight(viewportHeight) {}

void Camera::update(float targetX, float targetY, float mapWidthPx, float mapHeightPx) {
    // Center viewport on target
    x = targetX - (viewportWidth / 2.0f);
    y = targetY - (viewportHeight / 2.0f);

    // Clamp to map boundaries
    x = std::max(0.0f, std::min(x, mapWidthPx - viewportWidth));
    y = std::max(0.0f, std::min(y, mapHeightPx - viewportHeight));

}

void Camera::follow(float targetX, float targetY, float lerpSpeed, float mapWidthPx, float mapHeightPx) {
        
    float lookOffsetY = 0.0f;

    if (InputMap::isActionDown(Action::MoveUp) || InputMap::isActionHeld(Action::MoveUp))
    {
        lookOffsetY = -20.0f;
    }
    else if (InputMap::isActionDown(Action::MoveDown) || InputMap::isActionHeld(Action::MoveDown))
    {
        lookOffsetY = 20.0f;
    }

    float desiredX = targetX - (viewportWidth / 2.0f);
    float desiredY = targetY - (viewportHeight / 2.0f);

    // Smooth interpolation
    x += (desiredX - x) * lerpSpeed;
    y += (desiredY - y) * lerpSpeed;

    // Clamp to map boundaries
    x = std::max(0.0f, std::min(x, mapWidthPx - viewportWidth));
    y = std::max(0.0f, std::min(y, mapHeightPx - viewportHeight));
}