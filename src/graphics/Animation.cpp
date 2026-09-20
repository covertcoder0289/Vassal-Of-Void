#include "Animation.hpp"

Animation::Animation()
    : frameWidth(0),
      frameHeight(0),
      frameCount(0),
      currentFrame(0),
      frameTimer(0),
      frameDuration(8)
{
}

void Animation::setSpriteSheet(
    C2D_Image sheet,
    int width,
    int height,
    int count
)
{
    spriteSheet = sheet;

    frameWidth = width;
    frameHeight = height;
    frameCount = count;

    currentFrame = 0;
    frameTimer = 0;

    subTexture.width = width;
    subTexture.height = height;
}

void Animation::update()
{
    if (frameCount <= 1)
        return;

    frameTimer++;

    if (frameTimer >= frameDuration)
    {
        frameTimer = 0;

        currentFrame++;

        if (currentFrame >= frameCount)
            currentFrame = 0;
    }
}

void Animation::reset()
{
    currentFrame = 0;
    frameTimer = 0;
}

C2D_Image Animation::getCurrentFrame() const
{
    const Tex3DS_SubTexture* source = spriteSheet.subtex;

    float sheetWidthUV = source->right - source->left;
    float frameWidthUV = sheetWidthUV / frameCount;

    float left = source->left + currentFrame * frameWidthUV;
    float right = left + frameWidthUV;

    subTexture.width = frameWidth;
    subTexture.height = frameHeight;

    subTexture.left = left;
    subTexture.right = right;

    subTexture.top = source->top;
    subTexture.bottom = source->bottom;

    return C2D_Image{
        spriteSheet.tex,
        &subTexture
    };
}

void Animation::setFrameDuration(int duration)
{
    frameDuration = duration;
}