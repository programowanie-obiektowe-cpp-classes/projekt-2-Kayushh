#pragma once

#include <SFML/Graphics.hpp>
#include "Config.hpp"

bool isWall(int x, int y);
bool isPassable(int x, int y);
int wrapX(int x);
sf::Vector2f tileToPixelCenter(int tx, int ty);
sf::Vector2i pixelToTile(const sf::Vector2f& pos);
