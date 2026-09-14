//src/state/MenuState.cpp
#include "MenuState.hpp"
#include "common/colors.hpp"
#include <3ds.h>

MenuState::MenuState(StateManager& stateManager)
 : stateManager(stateManager) {
    textBuf = C2D_TextBufNew(4096);
    font = nullptr;

    items = {
        { "Start Game" , MenuAction::StartGame},
        { "Options", MenuAction::Options}
    };
}

MenuState::~MenuState() {
    C2D_TextBufDelete(textBuf);
}

void MenuState::update() {
    u32 kDown = hidKeysDown();

    if (kDown & KEY_DOWN) selectedIndex = (selectedIndex + 1) % items.size();
    if (kDown & KEY_UP)   selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
    
    if(kDown & KEY_A){
        switch(items[selectedIndex].action){
            case MenuAction::StartGame:
                stateManager.setState(std::make_unique<PlayState>(stateManager));
                break;
            case MenuAction::Options:
            //handle options menu here later
                break;

            default:
                //fallback action
                break;

        }
    }


}

void MenuState::drawTopScreen() {
    C2D_TextBufClear(textBuf);

    C2D_Text title;
    C2D_TextParse(&title, textBuf, "VASSAL OF VOID");
    C2D_TextOptimize(&title);
    C2D_DrawText(&title, C2D_AlignCenter | C2D_WithColor, 200.0f, 40.0f, 0.5f, 0.8f, 0.8f, C2D_Color32(255,255,255,255));

    float y = 120.0f;
    for (size_t i = 0; i < items.size(); ++i) {
        C2D_Text label;
        C2D_TextParse(&label, textBuf, items[i].label.c_str());
        C2D_TextOptimize(&label);

        u32 color = (i == (size_t)selectedIndex)
            ? Colors::mainMenuHighlight
            : Colors::lightGray;

        C2D_DrawText(&label, C2D_AlignCenter | C2D_WithColor, 200.0f, y, 0.5f, 0.5f, 0.5f, color);
        y += 24.0f;
    }
}

void MenuState::drawBottomScreen() {
    // empty for now
}