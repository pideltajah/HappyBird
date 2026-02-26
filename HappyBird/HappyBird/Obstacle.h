// Obstacle.h
// Justin Hampton July 9, 2014
// Updated for SFML 3.x

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <optional>
#include <string>
#include <SFML/Graphics.hpp>

constexpr float OBSTACLE_WIDTH  = 100.f;
constexpr float OBSTACLE_HEIGHT = 1600.f;
#ifndef VIEW_WIDTH
constexpr float VIEW_WIDTH = 1200.f;
#endif

class Obstacle {

private:
    sf::Texture obstacleTexture;
    std::optional<sf::Sprite> obstacle;

    float randNum;

public:
    Obstacle(const std::string& obstacleFile = "obstacle.bmp",
             float x = VIEW_WIDTH,
             float y = 0.f);

    void moveObstacle();

    const sf::Sprite& getObstacle() const;

    void initializeObstacle(float xPosition = VIEW_WIDTH);
};

#endif
