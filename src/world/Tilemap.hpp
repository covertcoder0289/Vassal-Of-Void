// src/world/Tilemap.hpp
#pragma once

#include <vector>
#include <citro2d.h>
#include "physics/AABB.hpp"

// Tile Types
enum TileType : int {
    AIR = 0,
    GROUND = 1,
    PLATFORM = 2
};

class Tilemap {
public:
    static constexpr float TILE_SIZE = 16.0f; // 16x16 fixed size

    Tilemap() = default;

    // Initialize map dimensions and fill data
    void loadMap(int columns, int rows, const std::vector<int>& mapData);

    void draw(float cameraX = 0.0f, float cameraY = 0.0f) const;

    // Helper functions for physics/collision
    int getTileAt(int col, int row) const;
    int getTileAtPosition(float worldX, float worldY) const;
    
    // Convert world position (pixels) to grid indices
    int worldToCol(float worldX) const { return static_cast<int>(worldX / TILE_SIZE); }
    int worldToRow(float worldY) const { return static_cast<int>(worldY / TILE_SIZE); }

    int getColumns() const { return mapColumns; }
    int getRows() const { return mapRows; }
    void setTileTexture(C2D_Image texture) { tileTexture = texture; }

private:
    int mapColumns = 0;
    int mapRows = 0;
    std::vector<int> tiles; // Grid stored as a 1D array
    C2D_Image tileTexture = {}; // Texture for rendering solid tiles
};