#include "Input.hpp"
#include "Config.hpp"

void updatePacmanInput(Pacman& pacman) {
    sf::Vector2f vel(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        vel.y = -PACMAN_SPEED;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        vel.y = PACMAN_SPEED;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        vel.x = -PACMAN_SPEED;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        vel.x = PACMAN_SPEED;
    }
    pacman.velocity = vel;
}
