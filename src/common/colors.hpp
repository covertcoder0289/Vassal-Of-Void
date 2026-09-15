//common/colors.hpp
#pragma once

#include <3ds.h>
#include <citro2d.h>

namespace Colors{
    inline constexpr u32 white = C2D_Color32(255,255,255,255);
    inline constexpr u32 mainMenuHighlight = C2D_Color32(255, 220, 150, 255);
    inline constexpr u32 lightGray = C2D_Color32(200, 200, 200, 255);
    inline constexpr u32 grayOverlay = C2D_Color32(180, 180, 180, 255);
    inline constexpr u32 pauseHighlight =  C2D_Color32(0, 212, 255, 255);
    inline constexpr u32 testGroundColor =  C2D_Color32(255, 220, 0, 255);
    inline constexpr u32 testPlatformColor =  C2D_Color32(255, 220, 0, 255);
    inline constexpr u32 green =  C2D_Color32(0, 255, 0, 255);
};