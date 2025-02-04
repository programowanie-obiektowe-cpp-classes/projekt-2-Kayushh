#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

float distanceBetween(const sf::Vector2f& a, const sf::Vector2f& b);
bool canMovePacman(const sf::Vector2f& newPos, float radius);
