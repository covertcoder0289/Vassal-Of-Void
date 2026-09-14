// src/states/PauseState.cpp
#include "PauseState.hpp"
#include "MenuState.hpp"
#include "common/colors.hpp"
#include "StateManager.hpp" // Included here to safely change states
#include <3ds.h>

PauseState::PauseState(StateManager& stateManager, std::function<void()> onResumeFn)
    : stateManager(stateManager), onResumeFn(std::move(onResumeFn)) {
    textBuf = C2D_TextBufNew(4096);
    font = nullptr;

    items = {
        { "Continue", MenuAction::Continue },
        { "Options", MenuAction::Options },
        { "Quit to Menu", MenuAction::QuitToMenu }
    };
}

PauseState::~PauseState() {
    C2D_TextBufDelete(textBuf);
}

void PauseState::update() {
    u32 kDown = hidKeysDown();

    if (kDown & KEY_DOWN) selectedIndex = (selectedIndex + 1) % items.size();
    if (kDown & KEY_UP)   selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
    
    if (kDown & KEY_A) {
        switch (items[selectedIndex].action) {
            case MenuAction::Continue:
                if (onResumeFn) onResumeFn();
                break;
                
            case MenuAction::Options:
                // Handle options overlay later
                break;
                
            case MenuAction::QuitToMenu:
                // FIX: Instantly routes back to MenuState using the stateManager reference
                stateManager.setState(std::make_unique<MenuState>(stateManager));
                break;
        }
    }
}

void PauseState::drawTopScreen() {
    C2D_TextBufClear(textBuf);

    C2D_Text title;
    C2D_TextParse(&title, textBuf, "PAUSED");
    C2D_TextOptimize(&title);
    C2D_DrawText(&title, C2D_AlignCenter | C2D_WithColor, 200.0f, 40.0f, 0.5f, 0.8f, 0.8f, C2D_Color32(255, 255, 255, 255));

    float y = 110.0f;
    for (size_t i = 0; i < items.size(); ++i) {
        C2D_Text label;
        C2D_TextParse(&label, textBuf, items[i].label.c_str());
        C2D_TextOptimize(&label);

        u32 color = (i == (size_t)selectedIndex)
            ? Colors::pauseHighlight // Vassal Blue highlight
            : Colors::grayOverlay;

        C2D_DrawText(&label, C2D_AlignCenter | C2D_WithColor, 200.0f, y, 0.5f, 0.5f, 0.5f, color);
        y += 24.0f;
    }
}

void PauseState::drawBottomScreen() {
    // Optional: Draw pause instructions on bottom screen
}
