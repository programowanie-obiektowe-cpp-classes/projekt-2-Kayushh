#include "Map.hpp"

bool isWall(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return true; // out of bounds => treat as wall
    }
    return (level[y][x] == '#');
}

bool isPassable(int x, int y) {
    return !isWall(x, y);
}

int wrapX(int x) {
    if (x < 0) {
        x = MAP_WIDTH - 1;
    }
    else if (x >= MAP_WIDTH) {
        x = 0;
    }
    return x;
}

sf::Vector2f tileToPixelCenter(int tx, int ty) {
    return sf::Vector2f((tx + 0.5f) * TILE_SIZE, (ty + 0.5f) * TILE_SIZE);
}

sf::Vector2i pixelToTile(const sf::Vector2f& pos) {
    return sf::Vector2i(
        static_cast<int>(pos.x / TILE_SIZE),
        static_cast<int>(pos.y / TILE_SIZE)
    );
}
