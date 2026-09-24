#include "world/RoomLoader.hpp"

#include <fstream>
#include <cstdlib> // std::strtof, std::strtol
#include <cctype>  // std::isspace


// Helper functions
static size_t findJsonKey(
    const std::string& content,
    const char* key,
    size_t startPos = 0
) {
    return content.find(key, startPos);
}


// Find ':' after a JSON key while allowing whitespace:
//
// "width":96
// "width": 96
// "width" : 96
// "width"    :    96
//
static size_t findJsonColon(
    const std::string& content,
    size_t keyPos
) {
    size_t pos = keyPos;

    while (pos < content.size()) {
        char c = content[pos];

        if (c == ':') {
            return pos;
        }

        // Don't accidentally cross too far into another property.
        if (c == ',' || c == '{' || c == '}') {
            return std::string::npos;
        }

        pos++;
    }

    return std::string::npos;
}


// Find the next non-whitespace character.
static size_t skipWhitespace(
    const std::string& content,
    size_t pos
) {
    while (pos < content.size() &&
           std::isspace(static_cast<unsigned char>(content[pos]))) {
        pos++;
    }

    return pos;
}


// Load room
bool RoomLoader::loadFromFile(
    const std::string& path,
    int& outCols,
    int& outRows,
    std::vector<int>& outData
) {
    std::ifstream file(path);
    if (!file.is_open()) {
        printf("ROOM ERROR: cannot open %s\n", path.c_str());
        return false;
    }

    outData.clear();
    outCols = 0;
    outRows = 0;

    std::string content(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();

    // -----------------------------------------------------
    // Locate the "layers" array boundaries FIRST, using real
    // bracket-depth tracking (not a naive find(']'), since layer
    // objects contain their own nested "data"/"objects" arrays).
    // Everything below searches for root properties OUTSIDE this
    // range, so a layer's — or a point object's — own width/height
    // can never be mistaken for the map's.
    // -----------------------------------------------------

    size_t layersKeyPos = content.find("\"layers\"");
    if (layersKeyPos == std::string::npos) {
        printf("ROOM ERROR: layers array not found\n");
        return false;
    }

    size_t layersColonPos = content.find(':', layersKeyPos);
    if (layersColonPos == std::string::npos) {
        printf("ROOM ERROR: layers colon not found\n");
        return false;
    }

    size_t layersArrayStart = skipWhitespace(content, layersColonPos + 1);
    if (layersArrayStart >= content.size() || content[layersArrayStart] != '[') {
        printf("ROOM ERROR: layers array '[' not found\n");
        return false;
    }

    int depth = 0;
    size_t layersArrayEnd = std::string::npos;
    for (size_t i = layersArrayStart; i < content.size(); ++i) {
        if (content[i] == '[') depth++;
        else if (content[i] == ']') {
            depth--;
            if (depth == 0) { layersArrayEnd = i; break; }
        }
    }
    if (layersArrayEnd == std::string::npos) {
        printf("ROOM ERROR: layers array ']' not found\n");
        return false;
    }

    // -----------------------------------------------------
    // Root map dimensions — height precedes "layers" alphabetically
    // in every Tiled export, width follows it. Searching each in
    // its own half of the file (outside the layers content entirely)
    // makes this correct regardless of layer order or count.
    // -----------------------------------------------------

    std::string beforeLayers = content.substr(0, layersKeyPos);
    std::string afterLayers  = content.substr(layersArrayEnd);

    size_t heightPos = beforeLayers.find("\"height\"");
    if (heightPos != std::string::npos) {
        size_t colonPos = beforeLayers.find(':', heightPos);
        if (colonPos != std::string::npos) {
            size_t valuePos = skipWhitespace(beforeLayers, colonPos + 1);
            outRows = static_cast<int>(std::strtol(beforeLayers.c_str() + valuePos, nullptr, 10));
        }
    }

    size_t widthPos = afterLayers.find("\"width\"");
    if (widthPos != std::string::npos) {
        size_t colonPos = afterLayers.find(':', widthPos);
        if (colonPos != std::string::npos) {
            size_t valuePos = skipWhitespace(afterLayers, colonPos + 1);
            outCols = static_cast<int>(std::strtol(afterLayers.c_str() + valuePos, nullptr, 10));
        }
    }

    if (outCols <= 0 || outRows <= 0) {
        printf("ROOM ERROR: invalid dimensions %d x %d\n", outCols, outRows);
        return false;
    }

    // ----- everything below here (tilelayer/data extraction) is
    // unchanged from what you already have — it forward-searches
    // for the tilelayer object specifically and was never actually
    // order-dependent, so it doesn't need touching.

    size_t tileLayerTypePos = content.find("\"type\":\"tilelayer\"");
    if (tileLayerTypePos == std::string::npos) {
        tileLayerTypePos = content.find("\"type\": \"tilelayer\"");
    }
    if (tileLayerTypePos == std::string::npos) {
        printf("ROOM ERROR: tilelayer not found\n");
        return false;
    }

    size_t layerStart = content.rfind('{', tileLayerTypePos);
    if (layerStart == std::string::npos) {
        printf("ROOM ERROR: tilelayer object start not found\n");
        return false;
    }

    size_t dataKeyPos = content.find("\"data\"", layerStart);
    if (dataKeyPos == std::string::npos || dataKeyPos > tileLayerTypePos) {
        printf("ROOM ERROR: data property not found in tilelayer\n");
        return false;
    }

    size_t dataColonPos = content.find(':', dataKeyPos);
    if (dataColonPos == std::string::npos) {
        printf("ROOM ERROR: data colon not found\n");
        return false;
    }

    size_t dataPos = skipWhitespace(content, dataColonPos + 1);
    if (dataPos >= content.size() || content[dataPos] != '[') {
        printf("ROOM ERROR: data array '[' not found\n");
        return false;
    }
    dataPos++;

    size_t dataEnd = content.find(']', dataPos);
    if (dataEnd == std::string::npos) {
        printf("ROOM ERROR: data array ']' not found\n");
        return false;
    }

    const char* ptr = content.c_str() + dataPos;
    const char* endPtr = content.c_str() + dataEnd;

    while (ptr < endPtr) {
        while (ptr < endPtr && (*ptr == ',' || std::isspace(static_cast<unsigned char>(*ptr)))) {
            ptr++;
        }
        if (ptr >= endPtr) break;

        char* nextPtr = nullptr;
        long value = std::strtol(ptr, &nextPtr, 10);

        if (ptr != nextPtr) {
            outData.push_back(static_cast<int>(value));
            ptr = nextPtr;
        } else {
            ptr++;
        }
    }

    if (outData.empty()) {
        printf("ROOM ERROR: no tile data parsed\n");
        return false;
    }

    size_t expectedSize = static_cast<size_t>(outCols) * static_cast<size_t>(outRows);
    if (outData.size() != expectedSize) {
        printf("ROOM ERROR: tile count mismatch!\n");
        return false;
    }

    return true;
}


// ---------------------------------------------------------
// Load spawn point
// ---------------------------------------------------------

bool RoomLoader::loadSpawnPoint(
    const std::string& path,
    float& outX,
    float& outY
) {
    std::ifstream file(path);

    if (!file.is_open()) {
        printf(
            "SPAWN ERROR: cannot open %s\n",
            path.c_str()
        );

        return false;
    }


    std::string content(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    file.close();


    // Find spawn_point layer
    size_t layerPos = content.find(
        "\"name\":\"spawn_point\""
    );

    if (layerPos == std::string::npos) {
        layerPos = content.find(
            "\"name\": \"spawn_point\""
        );
    }

    if (layerPos == std::string::npos) {
        printf("SPAWN ERROR: spawn_point layer not found\n");
        return false;
    }


    // Find objects
    size_t objectsPos = content.find(
        "\"objects\"",
        layerPos
    );

    if (objectsPos == std::string::npos) {
        printf("SPAWN ERROR: objects property not found\n");
        return false;
    }


    size_t objectsColon = findJsonColon(
        content,
        objectsPos
    );

    if (objectsColon == std::string::npos) {
        printf("SPAWN ERROR: objects colon not found\n");
        return false;
    }


    size_t objectArrayPos = skipWhitespace(
        content,
        objectsColon + 1
    );


    if (objectArrayPos >= content.size() ||
        content[objectArrayPos] != '[') {

        printf("SPAWN ERROR: objects array '[' not found\n");
        return false;
    }


    // Find first object inside objects array
    size_t objectStart = content.find(
        '{',
        objectArrayPos
    );

    if (objectStart == std::string::npos) {
        printf("SPAWN ERROR: spawn object not found\n");
        return false;
    }


    size_t objectEnd = content.find(
        '}',
        objectStart
    );

    if (objectEnd == std::string::npos) {
        printf("SPAWN ERROR: spawn object end not found\n");
        return false;
    }


    std::string objectBlock =
        content.substr(
            objectStart,
            objectEnd - objectStart
        );


    // X coordinate
    size_t xPos = objectBlock.find(
        "\"x\""
    );

    if (xPos == std::string::npos) {
        printf("SPAWN ERROR: x property not found\n");
        return false;
    }


    size_t xColon = findJsonColon(
        objectBlock,
        xPos
    );

    if (xColon == std::string::npos) {
        printf("SPAWN ERROR: x colon not found\n");
        return false;
    }


    size_t xValuePos = skipWhitespace(
        objectBlock,
        xColon + 1
    );


    // Y coordinate
    size_t yPos = objectBlock.find(
        "\"y\""
    );

    if (yPos == std::string::npos) {
        printf("SPAWN ERROR: y property not found\n");
        return false;
    }


    size_t yColon = findJsonColon(
        objectBlock,
        yPos
    );

    if (yColon == std::string::npos) {
        printf("SPAWN ERROR: y colon not found\n");
        return false;
    }


    size_t yValuePos = skipWhitespace(
        objectBlock,
        yColon + 1
    );


    // Convert coordinates
    outX = std::strtof(
        objectBlock.c_str() + xValuePos,
        nullptr
    );

    outY = std::strtof(
        objectBlock.c_str() + yValuePos,
        nullptr
    );


    printf("SPAWN LOAD SUCCESS: %.2f, %.2f\n",outX,outY);

    return true;
}