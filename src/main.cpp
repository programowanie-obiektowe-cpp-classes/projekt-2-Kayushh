#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Config.hpp"
#include "Map.hpp"
#include "Direction.hpp"
#include "Entities.hpp"
#include "Collision.hpp"
#include "AI.hpp"
#include "Input.hpp"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE),
        "Pac-Man");
    window.setFramerateLimit(60);

    // Load font 
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error: You must place arial.tff next to source files\n";
        return 1;
    }

    sf::Text scoreText, livesText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 0.f);

    livesText.setFont(font);
    livesText.setCharacterSize(18);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(200.f, 0.f);

    // Pac-man
    Pacman pacman;
    pacman.position = sf::Vector2f(0.f, 0.f);
    pacman.velocity = sf::Vector2f(0.f, 0.f);
    pacman.sprite.setRadius(10.f);
    pacman.sprite.setOrigin(10.f, 10.f);
    pacman.sprite.setFillColor(sf::Color::Yellow);
    pacman.alive = true;

    // Find Pac-man start in the map
    bool pacFound = false;
    for (int y = 0; y < MAP_HEIGHT && !pacFound; y++) {
        for (int x = 0; x < MAP_WIDTH && !pacFound; x++) {
            if (level[y][x] == 'P') {
                pacman.position = sf::Vector2f((x + 0.5f) * TILE_SIZE, (y + 0.5f) * TILE_SIZE);
                // remove 'P' from map
                level[y][x] = ' ';
                pacFound = true;
            }
        }
    }

    pacman.sprite.setPosition(pacman.position);

    // Ghosts
    std::vector<Ghost> ghosts;
    int ghostIndex = 0;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (level[y][x] == 'G') {
                Ghost g;
                g.ch.currentTile = { x, y };
                g.ch.nextTile = { x, y };
                g.ch.direction = Direction::None;
                g.ch.moveTimer = 0.f;
                g.ch.moveDuration = TIME_PER_TILE_GHOST;
                g.ch.alive = true;
                g.ch.sprite.setRadius(10.f);
                g.ch.sprite.setOrigin(10.f, 10.f);

                switch (ghostIndex % 4) {
                case 0: g.ch.sprite.setFillColor(sf::Color::Red);     break;
                case 1: g.ch.sprite.setFillColor(sf::Color::Cyan);    break;
                case 2: g.ch.sprite.setFillColor(sf::Color::Magenta); break;
                case 3: g.ch.sprite.setFillColor(sf::Color::Green);   break;
                }
                g.originalColor = g.ch.sprite.getFillColor();
                g.ch.sprite.setPosition(tileToPixelCenter(x, y));

                g.state = GhostState::Scatter;
                g.stateTimer = SCATTER_TIME;

                ghosts.push_back(g);
                ghostIndex++;
            }
        }
    }

    int score = 0;
    int lives = INITIAL_LIVES;
    bool gameOver = false;

    // A corner tile for scatter 
    sf::Vector2i scatterCorner(1, 1);

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
            }
        }

        // If game over
        if (gameOver) {
            window.clear(sf::Color::Black);
            sf::Text overText("YOU DIED", font, 32);
            overText.setFillColor(sf::Color::Red);
            overText.setPosition((MAP_WIDTH * TILE_SIZE) / 2.f - 80.f,
                (MAP_HEIGHT * TILE_SIZE) / 2.f - 20.f);
            window.draw(overText);
            window.display();
            continue;
        }

        // Update Pac-man if alive
        if (pacman.alive) {
            updatePacmanInput(pacman);

            // new position
            sf::Vector2f newPos = pacman.position + pacman.velocity * deltaTime;

            /* tunelling horizontal
            if (newPos.x < -TILE_SIZE) {
                newPos.x = MAP_WIDTH * TILE_SIZE;
            }
            else if (newPos.x > MAP_WIDTH * TILE_SIZE) {
                newPos.x = -TILE_SIZE;
            }
            */
            /*
            // tunelowanie pionowe
            if (newPos.y < -TILE_SIZE) {
                newPos.y = MAP_HEIGHT * TILE_SIZE;
            }
            else if (newPos.y > MAP_HEIGHT * TILE_SIZE) {
                newPos.y = -TILE_SIZE;
            }
            */

            // Check collision with walls
            float radius = pacman.sprite.getRadius();
            if (canMovePacman(newPos, radius)) {
                // move
                pacman.position = newPos;
                pacman.sprite.setPosition(pacman.position);

                // Check if Pac-man eats a dot in the tile he's in
                sf::Vector2i tileCoord = pixelToTile(pacman.position);
                if (tileCoord.x >= 0 && tileCoord.x < MAP_WIDTH &&
                    tileCoord.y >= 0 && tileCoord.y < MAP_HEIGHT)
                {
                    char& c = level[tileCoord.y][tileCoord.x];
                    if (c == '.') {
                        score += 10;
                        c = ' ';
                    }
                    else if (c == 'o') {
                        score += 50;
                        c = ' ';
                        // Turn ghosts frightened
                        for (auto& g : ghosts) {
                            g.state = GhostState::Frightened;
                            g.stateTimer = FRIGHTENED_TIME;
                            g.ch.moveDuration = TIME_PER_TILE_FRIGHTEN;
                            g.ch.sprite.setFillColor(sf::Color::Blue);
                        }
                    }
                }
            }
            else {

            }
        }

        // Update ghosts
        for (auto& g : ghosts) {
            g.stateTimer -= deltaTime;
            if (g.state == GhostState::Frightened) {
                if (g.stateTimer <= 0.f) {
                    g.state = GhostState::Scatter;
                    g.stateTimer = SCATTER_TIME;
                    g.ch.moveDuration = TIME_PER_TILE_GHOST;
                    g.ch.sprite.setFillColor(g.originalColor);
                }
            }
            else {
                // Scatter -> chase is on
                if (g.stateTimer <= 0.f) {
                    if (g.state == GhostState::Scatter) {
                        g.state = GhostState::Chase;
                        g.stateTimer = CHASE_TIME;
                    }
                    else {
                        g.state = GhostState::Scatter;
                        g.stateTimer = SCATTER_TIME;
                    }
                }
            }

            g.ch.moveTimer += deltaTime;
            if (g.ch.moveTimer >= g.ch.moveDuration) {
                g.ch.moveTimer = 0.f;
                g.ch.currentTile = g.ch.nextTile;

                // pick new direction
                Direction newDir = g.ch.direction;

                if (g.state == GhostState::Scatter) {
                    newDir = scatterAI(g.ch.currentTile, g.ch.direction, scatterCorner);
                }
                else if (g.state == GhostState::Chase && pacman.alive) {
                    // chase Pac-man tile
                    sf::Vector2i pacTile = pixelToTile(pacman.position);
                    newDir = chaseAI(g.ch.currentTile, g.ch.direction, pacTile);
                }
                else if (g.state == GhostState::Frightened) {
                    newDir = frightenedAI(g.ch.currentTile, g.ch.direction);
                }

                // attempt to move
                sf::Vector2i off = directionOffset(newDir);
                int nx = wrapX(g.ch.currentTile.x + off.x);
                int ny = g.ch.currentTile.y + off.y;
                if (isPassable(nx, ny)) {
                    g.ch.direction = newDir;
                    g.ch.nextTile = { nx, ny };
                }
                else {
                    // forced to reverse or do nothing
                    g.ch.direction = oppositeDirection(g.ch.direction);
                    sf::Vector2i revOff = directionOffset(g.ch.direction);
                    g.ch.nextTile = {
                        wrapX(g.ch.currentTile.x + revOff.x),
                        g.ch.currentTile.y + revOff.y
                    };
                }
            }

            // Interpolate ghost position
            float t = g.ch.moveTimer / g.ch.moveDuration;
            sf::Vector2f fromPos = tileToPixelCenter(g.ch.currentTile.x, g.ch.currentTile.y);
            sf::Vector2f toPos = tileToPixelCenter(g.ch.nextTile.x, g.ch.nextTile.y);
            sf::Vector2f pos = fromPos + (toPos - fromPos) * t;
            g.ch.sprite.setPosition(pos);
        }

        // collision pacman-ghost
        if (pacman.alive) {
            for (auto& g : ghosts) {
                if (!g.ch.alive) continue;
                float dist = distanceBetween(pacman.sprite.getPosition(), g.ch.sprite.getPosition());
                float sumR = pacman.sprite.getRadius() + g.ch.sprite.getRadius();
                if (dist < sumR) {
                    // If frightened = eat
                    if (g.state == GhostState::Frightened) {
                        score += 200;
                        // Ghost respawn
                        bool placed = false;
                        for (int yy = 0; yy < MAP_HEIGHT && !placed; yy++) {
                            for (int xx = 0; xx < MAP_WIDTH && !placed; xx++) {
                                if (level[yy][xx] == 'G') {
                                    g.ch.currentTile = { xx, yy };
                                    g.ch.nextTile = { xx, yy };
                                    g.ch.direction = Direction::None;
                                    g.ch.moveTimer = 0.f;
                                    g.ch.sprite.setPosition(tileToPixelCenter(xx, yy));
                                    g.state = GhostState::Scatter;
                                    g.stateTimer = SCATTER_TIME;
                                    g.ch.moveDuration = TIME_PER_TILE_GHOST;
                                    g.ch.sprite.setFillColor(g.originalColor);
                                    placed = true;
                                }
                            }
                        }
                    }
                    else {
                        // dying condition
                        lives--;
                        if (lives <= 0) {
                            gameOver = true;
                        }
                        else {
                            // Pacman respawns at some ' ' place 
                            bool foundSpot = false;
                            for (int yy = 0; yy < MAP_HEIGHT && !foundSpot; yy++) {
                                for (int xx = 0; xx < MAP_WIDTH && !foundSpot; xx++) {
                                    if (level[yy][xx] == ' ') {
                                        pacman.position = sf::Vector2f((xx + 0.5f) * TILE_SIZE, (yy + 0.5f) * TILE_SIZE);
                                        pacman.velocity = sf::Vector2f(0.f, 0.f);
                                        pacman.sprite.setPosition(pacman.position);
                                        foundSpot = true;
                                    }
                                }
                            }
                        }
                        pacman.alive = (lives > 0);
                        break; // break out of ghost loop
                    }
                }
            }
        }

        // Update UI
        scoreText.setString("Score: " + std::to_string(score));
        livesText.setString("Lives: " + std::to_string(lives));

        window.clear(sf::Color::Black);

        // Draw map
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                char c = level[y][x];
                if (c == '#') {
                    sf::RectangleShape wall(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    wall.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    wall.setFillColor(sf::Color::Blue);
                    window.draw(wall);
                }
                else if (c == '.') {
                    sf::CircleShape dot(3.f);
                    dot.setOrigin(3.f, 3.f);
                    dot.setFillColor(sf::Color::White);
                    dot.setPosition(x * TILE_SIZE + TILE_SIZE / 2.f,
                        y * TILE_SIZE + TILE_SIZE / 2.f);
                    window.draw(dot);
                }
                else if (c == 'o') {
                    sf::CircleShape power(6.f);
                    power.setOrigin(6.f, 6.f);
                    power.setFillColor(sf::Color::White);
                    power.setPosition(x * TILE_SIZE + TILE_SIZE / 2.f,
                        y * TILE_SIZE + TILE_SIZE / 2.f);
                    window.draw(power);
                }
            }
        }

        // Draw pacman if alive
        if (!gameOver && pacman.alive) {
            window.draw(pacman.sprite);
        }

        // Draw ghosts
        for (auto& g : ghosts) {
            window.draw(g.ch.sprite);
        }

        // UI
        window.draw(scoreText);
        window.draw(livesText);

        if (gameOver) {
            sf::Text overText("YOU DIED", font, 32);
            overText.setFillColor(sf::Color::Red);
            overText.setPosition((MAP_WIDTH * TILE_SIZE) / 2.f - 80.f,
                (MAP_HEIGHT * TILE_SIZE) / 2.f - 20.f);
            window.draw(overText);
        }

        window.display();
    }

    std::cout << "Score: " << score << std::endl;
    return 0;
}
