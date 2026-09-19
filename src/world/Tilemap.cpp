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

void Tilemap::draw(float cameraX, float cameraY) const {
    for (int row = 0; row < mapRows; ++row) {
        for (int col = 0; col < mapColumns; ++col) {
            int tileType = getTileAt(col, row);

            float screenX = (col * TILE_SIZE) - cameraX;
            float screenY = (row * TILE_SIZE) - cameraY;
            // Skip air tiles (ID 0)
            if (tileType == AIR){
                // Default color for unknown tile types
                u32 tileColor = Colors::mainMenuHighlight;
                C2D_DrawRectSolid(screenX, screenY, 0.5f, TILE_SIZE, TILE_SIZE, tileColor);
            }


            // Optional viewport culling: skip rendering tiles off-screen
            if (screenX + TILE_SIZE < 0 || screenX > 400.0f ||
                screenY + TILE_SIZE < 0 || screenY > 240.0f) {
                continue;
            }
            
            if (tileType == GROUND || tileType == PLATFORM) {
                C2D_DrawImageAt(tileTexture, screenX, screenY, 0.5f, nullptr, 1.0f, 1.0f);
            } 

        }
    }
}