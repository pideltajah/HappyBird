// Obstacle.cpp
// Justin Hampton July 9, 2014
// Updated for SFML 3.x

#include "Obstacle.h"
#include "HappyBird.h"
#include <iostream>
#include <ctime>

Obstacle::Obstacle(const std::string& obstacleFile, float x, float y) {

    if (!obstacleTexture.loadFromFile(obstacleFile)) {
        std::cout << "unable to load obstacleTexture from file" << std::endl;
    }

    obstacle.emplace(obstacleTexture);
    obstacle->setPosition({x, y});

    srand(static_cast<unsigned>(time(nullptr)));
    randNum = static_cast<float>(rand() % 800) - VIEW_HEIGHT;

} // end constructor

void Obstacle::moveObstacle() {

    if (obstacle->getPosition().x > -300.f) {
        obstacle->move({-2.f, 0.f});
    } else {
        randNum = static_cast<float>(rand() % 800) - VIEW_HEIGHT;
        obstacle->setPosition({VIEW_WIDTH, randNum});
    }

} // end moveObstacle()

const sf::Sprite& Obstacle::getObstacle() const {
    return *obstacle;
}

void Obstacle::initializeObstacle(float xPosition) {

    randNum = static_cast<float>(rand() % 800) - VIEW_HEIGHT;
    obstacle->setTexture(obstacleTexture);
    obstacle->setPosition({xPosition, randNum});

} // end initializeObstacle()
