#pragma once

#include <SFML/System/Vector2.hpp>
#include "Config.hpp"

enum class Direction { Up, Down, Left, Right, None };

sf::Vector2i directionOffset(Direction d);
Direction oppositeDirection(Direction d);
