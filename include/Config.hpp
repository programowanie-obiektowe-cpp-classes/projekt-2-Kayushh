#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

static const int    TILE_SIZE = 24;
static const int    MAP_WIDTH = 28;
static const int    MAP_HEIGHT = 30;

// pacman speed (pix/s)
static const float  PACMAN_SPEED = 80.f;

static const float  TIME_PER_TILE_GHOST = 0.3f;
static const float  TIME_PER_TILE_FRIGHTEN = 0.35f; // slower when frightened

static const float  SCATTER_TIME = 5.0f;
static const float  CHASE_TIME = 10.0f;
static const float  FRIGHTENED_TIME = 6.0f;

static const int    INITIAL_LIVES = 3;

// # = Wall
// . = dot 
// o = Powerup
// G = Ghost spawn
// P = Pacman start
// ' ' = empty

// Deklarujemy zmienn¹ globaln¹ – definicjê umieœcimy w Config.cpp
extern std::vector<std::string> level;
