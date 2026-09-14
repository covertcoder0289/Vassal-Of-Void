// src/world/Tilemap.cpp
#include "world/Tilemap.hpp"
#include "common/colors.hpp"

void Tilemap::loadMap(int columns, int rows, const std::vector<int>& mapData) {
    mapColumns = columns;
    mapRows = rows;
    tiles = mapData;
}

int Tilemap::getTileAt(int col, int row) const {
    // Check out-of-bounds boundary conditions
    if (col < 0 || col >= mapColumns || row < 0 || row >= mapRows) {
        return AIR;
    }
    return tiles[row * mapColumns + col]; // 1D array index math
}

int Tilemap::getTileAtPosition(float worldX, float worldY) const {
    int col = worldToCol(worldX);
    int row = worldToRow(worldY);
    return getTileAt(col, row);
}

void Tilemap::draw() const {
    for (int row = 0; row < mapRows; ++row) {
        for (int col = 0; col < mapColumns; ++col) {
            int tileType = getTileAt(col, row);

            // Skip air tiles (ID 0)
            if (tileType == AIR) continue;

            float drawX = col * TILE_SIZE;
            float drawY = row * TILE_SIZE;

            u32 tileColor = Colors::white;
            if (tileType == GROUND) {
                tileColor = Colors::testGroundColor; // Brown
            } else if (tileType == PLATFORM) {
                tileColor = Colors::testPlatformColor; // Yellow
            }

            C2D_DrawRectSolid(drawX, drawY, 0.5f, TILE_SIZE, TILE_SIZE, tileColor);
        }
    }
}