#include "InputMap.hpp"
#include "InputManager.hpp"
#include <3ds.h>

namespace {
    u32 keyFor(Action action) {
        switch (action) {
            case Action::Jump:      return KEY_B;
            case Action::Attack:    return KEY_Y;
            case Action::DreamNail: return KEY_X;
            case Action::Focus:     return KEY_A;
            case Action::Dash:      return KEY_R;
            case Action::SuperDash: return KEY_L;
            case Action::MoveLeft:  return KEY_DLEFT;
            case Action::MoveRight: return KEY_DRIGHT;
            case Action::MoveUp:    return KEY_DUP;
            case Action::MoveDown:  return KEY_DDOWN;
            case Action::Pause:     return KEY_START;
        }
        return 0;
    }
}

bool InputMap::isActionDown(Action action) { return InputManager::isKeyDown(keyFor(action)); }
bool InputMap::isActionHeld(Action action) { return InputManager::isKeyHeld(keyFor(action)); }
bool InputMap::isActionUp(Action action)   { return InputManager::isKeyUp(keyFor(action)); }

void InputMap::getMoveAxis(float& x, float& y) {
    float cx, cy;
    InputManager::getCirclePad(cx, cy);

    const float deadzone = 0.2f;
    if (cx > -deadzone && cx < deadzone) cx = 0.0f;
    if (cy > -deadzone && cy < deadzone) cy = 0.0f;

    x = cx;
    y = -cy; // circle pad dy is positive when pushed UP; flip so +y means "down" like D-Pad/screen space

    if (InputManager::isKeyHeld(KEY_DLEFT))  x = -1.0f;
    if (InputManager::isKeyHeld(KEY_DRIGHT)) x =  1.0f;
    if (InputManager::isKeyHeld(KEY_DUP))    y = -1.0f;
    if (InputManager::isKeyHeld(KEY_DDOWN))  y =  1.0f;
}