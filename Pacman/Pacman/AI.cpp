#include "AI.hpp"
#include "Direction.hpp"

std::vector<Direction> getValidDirections(const sf::Vector2i& tile, Direction currentDir) {
    std::vector<Direction> valid;
    for (auto d : { Direction::Up, Direction::Down, Direction::Left, Direction::Right }) {
        if (d == oppositeDirection(currentDir)) {
            continue; // don't reverse unless forced
        }
        sf::Vector2i off = directionOffset(d);
        int nx = wrapX(tile.x + off.x);
        int ny = tile.y + off.y;
        if (isPassable(nx, ny)) {
            valid.push_back(d);
        }
    }
    // If no valid directions except reverse, then ok.
    if (valid.empty()) {
        valid.push_back(oppositeDirection(currentDir));
    }
    return valid;
}

int manhattanDist(const sf::Vector2i& a, const sf::Vector2i& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

Direction scatterAI(const sf::Vector2i& ghostTile, Direction currentDir, const sf::Vector2i& corner) {
    auto dirs = getValidDirections(ghostTile, currentDir);
    // sort by manhattanDist to corner
    std::sort(dirs.begin(), dirs.end(), [&](Direction a, Direction b) {
        auto Aoff = directionOffset(a);
        auto Boff = directionOffset(b);
        sf::Vector2i nextA(wrapX(ghostTile.x + Aoff.x), ghostTile.y + Aoff.y);
        sf::Vector2i nextB(wrapX(ghostTile.x + Boff.x), ghostTile.y + Boff.y);
        return manhattanDist(nextA, corner) < manhattanDist(nextB, corner);
        });
    return dirs[0];
}

Direction chaseAI(const sf::Vector2i& ghostTile, Direction currentDir, const sf::Vector2i& pacTile) {
    auto dirs = getValidDirections(ghostTile, currentDir);
    std::sort(dirs.begin(), dirs.end(), [&](Direction a, Direction b) {
        auto Aoff = directionOffset(a);
        auto Boff = directionOffset(b);
        sf::Vector2i nextA(wrapX(ghostTile.x + Aoff.x), ghostTile.y + Aoff.y);
        sf::Vector2i nextB(wrapX(ghostTile.x + Boff.x), ghostTile.y + Boff.y);
        return manhattanDist(nextA, pacTile) < manhattanDist(nextB, pacTile);
        });
    return dirs[0];
}

Direction frightenedAI(const sf::Vector2i& ghostTile, Direction currentDir) {
    auto dirs = getValidDirections(ghostTile, currentDir);
    int r = std::rand() % dirs.size();
    return dirs[r];
}
