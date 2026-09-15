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

            if (tileType == GROUND || tileType == PLATFORM) {
                float posX = col * TILE_SIZE;
                float posY = row * TILE_SIZE;
                C2D_DrawImageAt(tileTexture, posX, posY, 0.5f, nullptr, 1.0f, 1.0f);
            } else {
                // Default color for unknown tile types
                u32 tileColor = Colors::grayOverlay;
                C2D_DrawRectSolid(drawX, drawY, 0.5f, TILE_SIZE, TILE_SIZE, tileColor);
            }

        }
    }
}