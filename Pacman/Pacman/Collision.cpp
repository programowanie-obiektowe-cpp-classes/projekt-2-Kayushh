#include "Collision.hpp"
#include "Config.hpp"
#include "Map.hpp"

float distanceBetween(const sf::Vector2f& a, const sf::Vector2f& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool canMovePacman(const sf::Vector2f& newPos, float radius) {
    // Check the 4 corners of pacman bounding box
    // If corner=wall, cannot move
    sf::Vector2f topLeft = { newPos.x - radius, newPos.y - radius };
    sf::Vector2f topRight = { newPos.x + radius, newPos.y - radius };
    sf::Vector2f bottomLeft = { newPos.x - radius, newPos.y + radius };
    sf::Vector2f bottomRight = { newPos.x + radius, newPos.y + radius };

    auto cornerToTile = [&](const sf::Vector2f& cpos) {
        return sf::Vector2i(
            static_cast<int>(cpos.x / TILE_SIZE),
            static_cast<int>(cpos.y / TILE_SIZE)
        );
        };

    sf::Vector2i tlTile = cornerToTile(topLeft);
    sf::Vector2i trTile = cornerToTile(topRight);
    sf::Vector2i blTile = cornerToTile(bottomLeft);
    sf::Vector2i brTile = cornerToTile(bottomRight);

    // Check each tile
    auto isTileWall = [&](sf::Vector2i tile) {
        // out of bounds => wall
        if (tile.x < 0 || tile.x >= MAP_WIDTH || tile.y < 0 || tile.y >= MAP_HEIGHT) {
            return true;
        }
        return (level[tile.y][tile.x] == '#');
        };

    if (isTileWall(tlTile) || isTileWall(trTile) ||
        isTileWall(blTile) || isTileWall(brTile)) {
        return false; // collision
    }

    return true; // no collision
}
