#pragma once

#include <SFML/Graphics.hpp>
#include "Direction.hpp"

struct Pacman {
    sf::Vector2f position;  // (x,y) in pixels
    sf::Vector2f velocity;  // (x,y) in pixels/sec
    sf::CircleShape sprite; // visual
    bool alive;
};

enum class GhostState {
    Scatter,
    Chase,
    Frightened
};

struct GhostChar {
    sf::Vector2i currentTile; // which tile we at
    sf::Vector2i nextTile;    // the tile we’re moving to
    Direction direction;       // current direction
    float moveTimer;           // how long it took to move from currenttile to nextTile
    float moveDuration;        // how long to move one tile
    sf::CircleShape sprite;
    bool alive;
};

struct Ghost {
    GhostChar ch;
    GhostState state;
    float stateTimer;
    sf::Color originalColor;
};
