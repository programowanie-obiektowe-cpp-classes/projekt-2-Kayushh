#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


static const int TILE_SIZE = 24;
static const int MAP_WIDTH = 28;
static const int MAP_HEIGHT = 30;
static const float SPEED = 2.0f;  
static const float GHOST_SPEED = 3.0f;  


// # = walls
// . = points
// G = ghosts
// o = fruit
std::vector<std::string> level = {
"############################",
"#............##............#",
"#.####.#####.##.#####.####.#",
"#o####.#####.##.#####.####o#",
"#.####.#####.##.#####.####.#",
"#..........................#",
"#.####.##.########.##.####.#",
"#.####.##.########.##.####.#",
"#......##....##....##......#",
"######.##### ## #####.######",
"######.##### ## #####.######",
"######.##          ##.######",
"######.## ###  ### ##.######",
"######.## # G  G # ##.######",
"######.## # G  G # ##.######",
"######.## ###  ### ##.######",
"######.##          ##.######",
"######.## ######## ##.######",
"######.## ######## ##.######",
"#............##............#",
"#.####.#####.##.#####.######",
"#.####.#####.##.#####.####.#",
"#o..##...... P .......##..o#",
"###.##.##.########.##.##.###",
"###.##.##.########.##.##.###",
"#......##....##....##......#",
"#.##########.##.##########.#",
"#.##########.##.##########.#",
"#..........................#",
"############################",
};

struct Entity {
    sf::Vector2f position;  
    sf::Vector2f velocity;  
    sf::CircleShape shape;    
    bool alive;
};


bool isWall(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return true; 
    }
    return (level[y][x] == '#');
}


sf::Vector2i getTileCoords(float x, float y) {
    return sf::Vector2i(static_cast<int>(x) / TILE_SIZE,
        static_cast<int>(y) / TILE_SIZE);
}


void checkCollisionWithWalls(Entity& entity) {
    sf::Vector2f newPos = entity.position + entity.velocity;
    sf::Vector2i tileCoordTopLeft = getTileCoords(newPos.x, newPos.y);
    sf::Vector2i tileCoordBottomRight = getTileCoords(newPos.x + entity.shape.getRadius() * 2,
        newPos.y + entity.shape.getRadius() * 2);
    if (isWall(tileCoordTopLeft.x, tileCoordTopLeft.y) ||
        isWall(tileCoordBottomRight.x, tileCoordTopLeft.y) ||
        isWall(tileCoordTopLeft.x, tileCoordBottomRight.y) ||
        isWall(tileCoordBottomRight.x, tileCoordBottomRight.y)) {
        entity.velocity.x = 0;
        entity.velocity.y = 0;
    }
}

void updateGhostMovement(Entity& ghost) {
    static int moveCounter = 0;
    static const int CHANGE_DIRECTION_INTERVAL = 60; 

    moveCounter--;
    if (moveCounter <= 0) {
        int dir = std::rand() % 4;
        switch (dir) {
        case 0: ghost.velocity = sf::Vector2f(GHOST_SPEED, 0);  break;  // prawo
        case 1: ghost.velocity = sf::Vector2f(-GHOST_SPEED, 0); break;  // lewo
        case 2: ghost.velocity =sf::Vector2f(0, GHOST_SPEED);  break;  // dol
        case 3: ghost.velocity = sf::Vector2f(0, -GHOST_SPEED); break;  // gora
        }
        moveCounter = CHANGE_DIRECTION_INTERVAL;
    }
}

bool checkCollisionPacmanGhost(const Entity& pacman, const Entity& ghost) {
    float dx = pacman.position.x - ghost.position.x;
    float dy = pacman.position.y - ghost.position.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    float radiusSum = pacman.shape.getRadius() + ghost.shape.getRadius();
    return (distance < radiusSum);
}

/*int Taxicab(const sf::Vector2i& a, const sf::Vector2i& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

Direction chase(const sf::Vector2i
*/

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE),
        "pm",
        sf::Style::Close);
    window.setFramerateLimit(60);

    Entity pacman;
    pacman.shape.setRadius(10.f);
    pacman.shape.setFillColor(sf::Color::Yellow);
    pacman.alive = true;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (level[y][x] == '.') {
                pacman.position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
                goto foundPacmanStart; 
            }
        }
    }
foundPacmanStart:;

    std::vector<Entity> ghosts;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (level[y][x] == 'G') {
                Entity ghost;
                ghost.shape.setRadius(10.f);
                ghost.shape.setFillColor(sf::Color::Red);
                ghost.position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
                ghost.velocity = sf::Vector2f(0, 0);
                ghost.alive = true;
                ghosts.push_back(ghost);
            }
        }
    }
    int score = 0;



    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (pacman.alive) {
            // controls 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                pacman.velocity.x = -SPEED;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                pacman.velocity.x = SPEED;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                pacman.velocity.y = -SPEED;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                pacman.velocity.y = SPEED;
            }
            

            
            checkCollisionWithWalls(pacman);
            pacman.velocity = sf::Vector2f(0.f, 0.f);
            pacman.position += pacman.velocity;
        }

        for (auto& ghost : ghosts) {
            if (!ghost.alive) continue;
            updateGhostMovement(ghost);
            checkCollisionWithWalls(ghost);
            ghost.position += ghost.velocity;
        }

        if (pacman.alive) {
            for (auto& ghost : ghosts) {
                if (ghost.alive) {
                    if (checkCollisionPacmanGhost(pacman, ghost)) {
                        pacman.alive = false;
                        std::cout << "You died.\n";
                        break;
                    }
                }
            }
        }

        if (pacman.alive) {
            sf::Vector2i tileCoord = getTileCoords(pacman.position.x + pacman.shape.getRadius(),
                pacman.position.y + pacman.shape.getRadius());
            if (level[tileCoord.y][tileCoord.x] == '.') {
                level[tileCoord.y][tileCoord.x] = ' '; 
                score += 10;
            }
        }

        window.clear(sf::Color::Black);

        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (level[y][x] == '#') {
                    sf::RectangleShape wall(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    wall.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    wall.setFillColor(sf::Color::Blue);
                    window.draw(wall);
                }
                else if (level[y][x] == '.') {
                    sf::CircleShape dot(3.f);
                    dot.setFillColor(sf::Color::White);
                    dot.setPosition(x * TILE_SIZE + TILE_SIZE / 2 - 3,
                        y * TILE_SIZE + TILE_SIZE / 2 - 3);
                    window.draw(dot);
                }
            }
        }

        if (pacman.alive) {
            pacman.shape.setPosition(pacman.position);
            window.draw(pacman.shape);
        }

        for (auto& ghost : ghosts) {
            if (!ghost.alive) continue;
            ghost.shape.setPosition(ghost.position);
            window.draw(ghost.shape);
        }

        // wynik
        // font
        std::cout << "\rScore: " << score << std::flush;

        window.display();
    }

    std::cout << "\nScore: " << score << std::endl;
    return 0;
}





/*sf::Font font;
if (!font.loadFromFile("")) {
    std::cerr << "Err\n";
    return 1;
}
*/






/*int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
 
    // Load a sprite to display
    const sf::Texture texture("cute_image.jpg");
    sf::Sprite sprite(texture);
 
    // Create a graphical text to display
    const sf::Font font("arial.ttf");
    sf::Text text(font, "Hello SFML", 50);
 
    // Load a music to play
    sf::Music music("nice_music.ogg");
 
    // Play the music
    music.play();
 
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }
 
        // Clear screen
        window.clear();
 
        // Draw the sprite
        window.draw(sprite);
 
        // Draw the string
        window.draw(text);
 
        // Update the window
        window.display();
    }
}
*/