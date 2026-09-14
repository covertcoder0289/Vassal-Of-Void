//src/physics/AABB.cpp
#include "AABB.hpp"

bool aabbOverlap(const AABB& a, const AABB& b) {
    return a.left() < b.right() && a.right() > b.left() &&
           a.top() < b.bottom() && a.bottom() > b.top();
}