#include <string>
#include <vector>

class RoomLoader {
public:
static bool loadFromFile(const std::string& path, int& outCols, int& outRows, std::vector<int>& outData);

    // Parses the object layer named "spawn_point" and retrieves its x and y coordinates.
static bool loadSpawnPoint(const std::string& path, float& outX, float& outY);
};
