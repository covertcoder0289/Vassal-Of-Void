#pragma once
#include <3ds.h>

class InputManager {
public:
    static void update(); // call once per frame, right after hidScanInput()

    static bool isKeyDown(u32 key); // pressed this frame
    static bool isKeyHeld(u32 key); // held this frame
    static bool isKeyUp(u32 key);   // released this frame

    static void getCirclePad(float& x, float& y); // normalized -1..1
};