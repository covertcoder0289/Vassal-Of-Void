//src/main.cpp
#include <citro2d.h>
#include <3ds.h>
#include <memory>
#include "debug/Debug.hpp"

#include "state/StateManager.hpp"
#include "state/MenuState.hpp"
#include "core/InputManager.hpp"

int main(int argc, char* argv[]) {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    

    Result romfsResult = romfsInit();
    ASSERT(R_SUCCEEDED(romfsResult), "romfs initialisation error");

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    C3D_RenderTarget* top    = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    //C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    StateManager stateManager;
    stateManager.setState(std::make_unique<MenuState>(stateManager));

    while (aptMainLoop()) {
        InputManager::update(); //contains hidScanInput() because the I need to call update in main otherwise user input wont work. This forces me to not leave out this step 

        stateManager.update();
        u32 kDown = hidKeysDown();
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        C2D_TargetClear(top, C2D_Color32(20, 20, 30, 255));
        C2D_SceneBegin(top);
        stateManager.drawTopScreen();
        //C2D_TargetClear(bottom, C2D_Color32(10, 10, 15, 255));
        //C2D_SceneBegin(bottom);
        //Draw bottom screen
        //stateManager.drawBottomScreen();
        C3D_FrameEnd(0);
    }

    C2D_Fini();
    C3D_Fini();
    romfsExit();
    gfxExit();
    return 0;
}