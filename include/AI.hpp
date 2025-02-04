#pragma once

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include "Direction.hpp"
#include "Map.hpp"

std::vector<Direction> getValidDirections(const sf::Vector2i& tile, Direction currentDir);
int manhattanDist(const sf::Vector2i& a, const sf::Vector2i& b);
Direction scatterAI(const sf::Vector2i& ghostTile, Direction currentDir, const sf::Vector2i& corner);
Direction chaseAI(const sf::Vector2i& ghostTile, Direction currentDir, const sf::Vector2i& pacTile);
Direction frightenedAI(const sf::Vector2i& ghostTile, Direction currentDir);
