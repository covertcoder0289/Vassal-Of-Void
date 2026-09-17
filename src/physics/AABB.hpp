//src/physics/AABB.hpp
#pragma once

struct AABB {
    float xPosition, yPosition, width, height;

    float left()   const { return xPosition; }
    float right()  const { return xPosition + width; }
    float top()    const { return yPosition; }
    float bottom() const { return yPosition + height; }
    float bottomRight() const { return yPosition + height + width; }
};

bool aabbOverlap(const AABB& a, const AABB& b);