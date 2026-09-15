// src/world/RoomLoader.cpp
#include "world/RoomLoader.hpp"
#include <cstdio>
#include <sstream>

bool RoomLoader::loadFromFile(const std::string& path, int& outCols, int& outRows, std::vector<int>& outData) {
    FILE* file = fopen(path.c_str(), "r");
    if (!file) return false;

    outData.clear();
    outRows = 0;
    outCols = 0;

    char lineBuffer[4096];
    while (fgets(lineBuffer, sizeof(lineBuffer), file)) {
        std::stringstream lineStream(lineBuffer);
        std::string cell;
        int colsThisRow = 0;

        while (std::getline(lineStream, cell, ',')) {
            outData.push_back(std::stoi(cell));
            colsThisRow++;
        }

        if (colsThisRow > 0) {
            outCols = colsThisRow; // assumes every row has the same width
            outRows++;
        }
    }

    fclose(file);
    return outRows > 0 && outCols > 0;
}