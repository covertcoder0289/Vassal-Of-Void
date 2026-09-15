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
    // Find root map dimensions
    // -----------------------------------------------------
    //
    // Use the LAST width/height because other objects/layers
    // can also contain width/height properties.
    //

    size_t widthPos = content.rfind("\"width\"");

    if (widthPos != std::string::npos) {

        size_t colonPos = content.find(':', widthPos);

        if (colonPos != std::string::npos) {

            size_t valuePos = colonPos + 1;

            while (
                valuePos < content.size() &&
                std::isspace(
                    static_cast<unsigned char>(
                        content[valuePos]
                    )
                )
            ) {
                valuePos++;
            }

            outCols = static_cast<int>(
                std::strtol(
                    content.c_str() + valuePos,
                    nullptr,
                    10
                )
            );
        }
    }


    size_t heightPos = content.rfind("\"height\"");

    if (heightPos != std::string::npos) {

        size_t colonPos = content.find(':', heightPos);

        if (colonPos != std::string::npos) {

            size_t valuePos = colonPos + 1;

            while (
                valuePos < content.size() &&
                std::isspace(
                    static_cast<unsigned char>(
                        content[valuePos]
                    )
                )
            ) {
                valuePos++;
            }

            outRows = static_cast<int>(
                std::strtol(
                    content.c_str() + valuePos,
                    nullptr,
                    10
                )
            );
        }
    }


    if (outCols <= 0 || outRows <= 0) {

        printf(
            "ROOM ERROR: invalid dimensions %d x %d\n",
            outCols,
            outRows
        );

        return false;
    }


    // Find tile layer
    size_t tileLayerTypePos =
        content.find("\"type\":\"tilelayer\"");

    if (tileLayerTypePos == std::string::npos) {

        tileLayerTypePos =
            content.find("\"type\": \"tilelayer\"");
    }

    if (tileLayerTypePos == std::string::npos) {

        printf("ROOM ERROR: tilelayer not found\n");
        return false;
    }


    // -----------------------------------------------------
    // Find the beginning of THIS layer object
    // -----------------------------------------------------
    //
    // The "data" property can appear before "type".
    //
    // Example:
    //
    // {
    //     "data":[...],
    //     "height":52,
    //     ...
    //     "type":"tilelayer"
    // }
    //
    // So we must search backwards to the '{' belonging
    // to this layer.
    //

    size_t layerStart =
        content.rfind('{', tileLayerTypePos);

    if (layerStart == std::string::npos) {

        printf("ROOM ERROR: tilelayer object start not found\n");
        return false;
    }


    // Find "data" inside this layer
    size_t dataKeyPos =
        content.find("\"data\"", layerStart);

    if (
        dataKeyPos == std::string::npos ||
        dataKeyPos > tileLayerTypePos
    ) {

        printf(
            "ROOM ERROR: data property not found in tilelayer\n"
        );

        return false;
    }

    // Find ':' after "data"
    size_t dataColonPos =
        content.find(':', dataKeyPos);

    if (dataColonPos == std::string::npos) {

        printf("ROOM ERROR: data colon not found\n");
        return false;
    }


    // Skip whitespace
    size_t dataPos =
        dataColonPos + 1;

    while (
        dataPos < content.size() &&
        std::isspace(
            static_cast<unsigned char>(
                content[dataPos]
            )
        )
    ) {
        dataPos++;
    }

    // Verify '['
    if (
        dataPos >= content.size() ||
        content[dataPos] != '['
    ) {

        printf(
            "ROOM ERROR: data array '[' not found\n"
        );

        return false;
    }
    // Move past '['
    dataPos++;


    // Find closing ']'
    size_t dataEnd =
        content.find(']', dataPos);

    if (dataEnd == std::string::npos) {

        printf(
            "ROOM ERROR: data array ']' not found\n"
        );

        return false;
    }

    // Parse tile indices
    const char* ptr =
        content.c_str() + dataPos;

    const char* endPtr =
        content.c_str() + dataEnd;

    while (ptr < endPtr) {

        // Skip commas and whitespace
        while (
            ptr < endPtr &&
            (
                *ptr == ',' ||
                std::isspace(
                    static_cast<unsigned char>(*ptr)
                )
            )
        ) {
            ptr++;
        }

        if (ptr >= endPtr) {
            break;
        }

        char* nextPtr = nullptr;
        long value =
            std::strtol(
                ptr,
                &nextPtr,
                10
            );

        if (ptr != nextPtr) {
            outData.push_back(
                static_cast<int>(value)
            );
            ptr = nextPtr;
        } else {
            // Prevent infinite loop if something unexpected
            // appears in the data array.
            ptr++;
        }
    }

    // Validate tile count
    if (outData.empty()) {

        printf(
            "ROOM ERROR: no tile data parsed\n"
        );

        return false;
    }

    size_t expectedSize =
        static_cast<size_t>(outCols) *
        static_cast<size_t>(outRows);


    // printf(
    //     "ROOM LOAD DATA:\n"
    //     "  dimensions: %d x %d\n"
    //     "  expected tiles: %zu\n"
    //     "  parsed tiles:   %zu\n",
    //     outCols,
    //     outRows,
    //     expectedSize,
    //     outData.size()
    // );

    if (outData.size() != expectedSize) {

        printf(
            "ROOM ERROR: tile count mismatch!\n"
        );

        return false;
    }
    //printf("ROOM LOAD SUCCESS\n");

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