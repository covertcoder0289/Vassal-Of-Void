#pragma once

enum class Action {
    Jump,
    Attack,
    DreamNail,
    Focus,
    Dash,
    SuperDash,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Pause
};

namespace InputMap {
    bool isActionDown(Action action); // pressed this frame
    bool isActionHeld(Action action); // held this frame
    bool isActionUp(Action action);   // released this frame

    // Combines D-Pad + circle pad into one -1..1 axis per direction
    void getMoveAxis(float& x, float& y);
}