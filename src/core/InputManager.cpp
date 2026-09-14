#include "InputManager.hpp"

namespace {
    u32 heldKeys = 0;
    u32 downKeys = 0;
    u32 upKeys   = 0;
    circlePosition circlePos;
}

void InputManager::update() {
    hidScanInput();
    heldKeys = hidKeysHeld();
    downKeys = hidKeysDown();
    upKeys   = hidKeysUp();
    hidCircleRead(&circlePos);
}

bool InputManager::isKeyDown(u32 key) { return downKeys & key; }
bool InputManager::isKeyHeld(u32 key) { return heldKeys & key; }
bool InputManager::isKeyUp(u32 key)   { return upKeys & key; }

void InputManager::getCirclePad(float& x, float& y) {
    x = circlePos.dx / 156.0f;
    y = circlePos.dy / 156.0f;
    if (x >  1.0f) x =  1.0f;
    if (x < -1.0f) x = -1.0f;
    if (y >  1.0f) y =  1.0f;
    if (y < -1.0f) y = -1.0f;
}