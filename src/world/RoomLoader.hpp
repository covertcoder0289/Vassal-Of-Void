// src/world/RoomLoader.hpp
#pragma once
#include <vector>
#include <string>

namespace RoomLoader {
    // Reads a comma-separated grid from a RomFS path.
    // Returns false if the file couldn't be opened.
    bool loadFromFile(const std::string& path, int& outCols, int& outRows, std::vector<int>& outData);
}