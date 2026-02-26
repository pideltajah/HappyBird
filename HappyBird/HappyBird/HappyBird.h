// HappyBird.h
// Justin Hampton 4/23/14
// Updated for SFML 3.x

#ifndef HAPPYBIRD_H
#define HAPPYBIRD_H

#include <optional>
#include <string>
#include "Bird.h"
#include "Obstacle.h"

// VIEW_WIDTH (1200.f) comes from Obstacle.h
constexpr float VIEW_HEIGHT = 800.f;

class HappyBird {

private:
    Bird bird;
    Obstacle obstacle;
    Obstacle obstacle2;
    Obstacle bonus;

    bool flapping;
    bool firstFrame;
    bool birdDied;
    bool dying;
    bool paused;
    bool gameIsOver;
    bool startMusic;

    sf::Clock flapClock;
    sf::Clock animationClock;
    sf::Clock pauseClock;
    sf::Clock musicClock;

    sf::Time flapTime;
    sf::Time animationTime;
    sf::Time totalFlapTime;
    sf::Time totalAnimationTime;
    sf::Time pauseTime;
    sf::Time musicTime;
    sf::Time totalMusicTime;

    sf::Texture backgroundTexture;
    sf::Texture gameOverTexture;

    std::optional<sf::Sprite> background;

    int score;
    int lives;
    int obstacleCount;

public:
    HappyBird();

    void play();
    void initialize();
    void flap(sf::Time flapTime);
    void dontFlap();

    bool getFlapping();
    void setFlapping(bool flapping);

    sf::Time getFlapTime();
    sf::Time getAnimationTime();

    void setFlapTime(sf::Time flapTime);
    void setAnimationTime(sf::Time animationTime);

    sf::Time getTotalFlapTime();
    sf::Time getTotalAnimationTime();

    int getLives();

    std::string intToString(int num);

    Bird getBird();

    void killBird();
    void revive();
    void pause();
    void restart();
    void gameOver();
};

#endif
