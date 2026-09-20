#pragma once

#include <3ds.h>
#include <citro2d.h>

class Animation
{
public:
    Animation();

    void setSpriteSheet(
        C2D_Image sheet,
        int frameWidth,
        int frameHeight,
        int frameCount
    );

    void update();
    void reset();

    void setFrameDuration(int duration);

    C2D_Image getCurrentFrame() const;

private:
    C2D_Image spriteSheet;

    mutable Tex3DS_SubTexture subTexture;

    int frameWidth;
    int frameHeight;
    int frameCount;

    int currentFrame;
    int frameTimer;
    int frameDuration;
};