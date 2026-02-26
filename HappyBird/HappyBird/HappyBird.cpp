// HappyBird.cpp
// Justin Hampton 4/23/14
// Updated for SFML 3.x

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <sstream>
#include <iostream>
#include "HappyBird.h"
#include <string>

// Default constructor
HappyBird::HappyBird() {

    bird      = Bird();
    obstacle  = Obstacle();
    obstacle2 = Obstacle("obstacle2.bmp");
    bonus     = Obstacle("volcano.bmp");

    flapping   = false;
    firstFrame = true;
    birdDied   = false;
    dying      = true;
    paused     = false;
    gameIsOver = false;
    startMusic = true;

    flapClock      = sf::Clock();
    animationClock = sf::Clock();
    pauseClock     = sf::Clock();
    musicClock     = sf::Clock();

    flapTime            = sf::Time();
    animationTime       = sf::Time();
    totalFlapTime       = sf::milliseconds(300);
    totalAnimationTime  = sf::milliseconds(3);
    pauseTime           = sf::Time();
    musicTime           = sf::Time();
    totalMusicTime      = sf::seconds(95);

    if (!backgroundTexture.loadFromFile("background.bmp")) {
        std::cout << "Error: unable to load background texture" << std::endl;
    }
    if (!gameOverTexture.loadFromFile("gameOver.bmp")) {
        std::cout << "Error: unable to load gameOver texture" << std::endl;
    }

    background.emplace(backgroundTexture);

    score         = 0;
    lives         = 3;
    obstacleCount = 0;

} // end default constructor

void HappyBird::play() {

    sf::Texture cloud1Texture;
    sf::Texture cloud2Texture;
    sf::Texture cloud3Texture;
    sf::Texture mountainTexture;

    if (!cloud1Texture.loadFromFile("cloud1.bmp"))
        std::cout << "Error: unable to load cloud1Texture" << std::endl;
    if (!cloud2Texture.loadFromFile("cloud2.bmp"))
        std::cout << "Error: unable to load cloud2Texture" << std::endl;
    if (!cloud3Texture.loadFromFile("cloud3.bmp"))
        std::cout << "Error: unable to load cloud3Texture" << std::endl;
    if (!mountainTexture.loadFromFile("mountains.bmp"))
        std::cout << "Error: unable to load mountainTexture" << std::endl;

    sf::Sprite cloud1(cloud1Texture);
    sf::Sprite cloud2(cloud2Texture);
    sf::Sprite cloud3(cloud3Texture);
    sf::Sprite mountains(mountainTexture);
    sf::Sprite mountains2(mountainTexture);

    cloud1.setPosition({VIEW_WIDTH, 73.f});
    cloud2.setPosition({VIEW_WIDTH, 79.f});
    cloud3.setPosition({0.f, 75.f});
    mountains.setPosition({0.f, 380.f});
    mountains2.setPosition({1200.f, 380.f});

    float birdFront, birdBack, birdHead, birdFoot;
    float obstacleFront, obstacleBack, obstacleTop, obstacleBottom;
    float obstacle2Front, obstacle2Back, obstacle2Top, obstacle2Bottom;

    sf::Font font;
    if (!font.openFromFile("Bicubik.OTF")) {
        std::cout << "unable to load font" << std::endl;
    }

    sf::Text hud(font, "", 14);
    hud.setPosition({VIEW_WIDTH - 200.f, 2.f});
    hud.setFillColor(sf::Color::Red);
    hud.setString("Score: " + std::to_string(score) + "\nLives: " + std::to_string(lives));
    hud.setStyle(sf::Text::Regular);

    sf::SoundBuffer scoreSound;
    sf::SoundBuffer backgroundMusic;

    if (!scoreSound.loadFromFile("score.wav")) {
        std::cout << "unable to open score.wav" << std::endl;
        exit(1);
    }
    if (!backgroundMusic.loadFromFile("background.wav")) {
        std::cout << "unable to open background.wav" << std::endl;
        exit(1);
    }

    sf::Sound scoreSFX(scoreSound);
    sf::Sound backgroundSFX(backgroundMusic);

    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned>(VIEW_WIDTH), static_cast<unsigned>(VIEW_HEIGHT)}),
        "Happy Bird!",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setVerticalSyncEnabled(true);

    initialize();

    while (window.isOpen()) {

        if (paused) {
            pauseTime += pauseClock.getElapsedTime();
            backgroundSFX.pause();
        }

        musicTime = musicClock.getElapsedTime() - pauseTime;

        if (backgroundSFX.getPlayingOffset() >= totalMusicTime) {
            startMusic = true;
            musicClock.restart();
        }

        if (startMusic) {
            backgroundSFX.play();
            startMusic = false;
        }

        // Process events
        while (const std::optional<sf::Event> event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                exit(0);
            }

            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    if (!paused) {
                        flapClock.restart();
                        animationClock.restart();
                        setFlapping(true);
                        firstFrame = true;
                        bird.playFlapSound();
                    }
                }
            }

            if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>()) {
                if (keyEvent->code == sf::Keyboard::Key::P) {
                    pause();
                } else if (keyEvent->code == sf::Keyboard::Key::R) {
                    restart();
                }
            }
        }

        hud.setString("Score: " + std::to_string(score) + "\nLives: " + std::to_string(lives));

        if (!paused) {

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                bird.dive();
            } else {
                bird.stopDive();
            }

            setFlapTime(flapClock.getElapsedTime());
            setAnimationTime(animationClock.getElapsedTime());

            if (flapping) {
                flap(flapTime);
            } else {
                dontFlap();
            }

            // Move clouds
            if (cloud1.getPosition().x > -300.f)
                cloud1.move({-0.5f, 0.f});
            else
                cloud1.setPosition({VIEW_WIDTH, 73.f});

            if (cloud2.getPosition().x > -300.f)
                cloud2.move({-1.0f, 0.f});
            else
                cloud2.setPosition({VIEW_WIDTH, 79.f});

            if (cloud3.getPosition().x > -300.f)
                cloud3.move({-1.5f, 0.f});
            else
                cloud3.setPosition({VIEW_WIDTH, 150.f});

            // Move mountains
            if (mountains.getPosition().x > -1200.f)
                mountains.move({-0.75f, 0.f});
            else
                mountains.setPosition({1200.f, 380.f});

            if (mountains2.getPosition().x > -1200.f)
                mountains2.move({-0.75f, 0.f});
            else
                mountains2.setPosition({1200.f, 380.f});

            obstacle.moveObstacle();
            obstacle2.moveObstacle();

            birdFront = bird.getBird().getPosition().x + BIRD_WIDTH;
            birdBack  = bird.getBird().getPosition().x;
            birdHead  = bird.getBird().getPosition().y;
            birdFoot  = bird.getBird().getPosition().y + (BIRD_HEIGHT * 0.5f);

            obstacleFront  = obstacle.getObstacle().getPosition().x;
            obstacleBack   = obstacle.getObstacle().getPosition().x + OBSTACLE_WIDTH;
            obstacleTop    = obstacle.getObstacle().getPosition().y + 750.f;
            obstacleBottom = obstacle.getObstacle().getPosition().y + 850.f;

            obstacle2Front  = obstacle2.getObstacle().getPosition().x;
            obstacle2Back   = obstacle2.getObstacle().getPosition().x + OBSTACLE_WIDTH;
            obstacle2Top    = obstacle2.getObstacle().getPosition().y + 750.f;
            obstacle2Bottom = obstacle2.getObstacle().getPosition().y + 850.f;

            if (birdFront > obstacleFront && birdBack < obstacleBack &&
                (birdHead < obstacleTop || birdFoot > obstacleBottom)) {
                killBird();
            }

            if (birdBack == obstacleBack && !birdDied) {
                scoreSFX.play();
                ++score;
            }

            if (birdFront > obstacle2Front && birdBack < obstacle2Back &&
                (birdHead < obstacle2Top || birdFoot > obstacle2Bottom)) {
                killBird();
            }

            if (birdBack == obstacle2Back && !birdDied) {
                scoreSFX.play();
                ++score;
            }

        } // end if not paused

        if (bird.getIsDead() && !gameIsOver) {
            bird.die();
        } else if (lives < 1 && !bird.getIsDead()) {
            gameOver();
        } else if (birdDied && !gameIsOver && !bird.getIsDead()) {
            revive();
        }

        window.clear(sf::Color::Blue);

        window.draw(*background);

        if (!gameIsOver) {
            window.draw(mountains);
            window.draw(mountains2);
            window.draw(cloud1);
            window.draw(cloud2);
            window.draw(cloud3);
            window.draw(bird.getBird());
            window.draw(obstacle.getObstacle());
            window.draw(obstacle2.getObstacle());
        }

        window.draw(hud);
        window.display();

    } // end main loop

} // end play()

void HappyBird::initialize() {

    gameIsOver = false;
    startMusic = true;

    background->setTexture(backgroundTexture);

    flapClock.restart();
    animationClock.restart();
    musicClock.restart();

    setFlapping(true);
    firstFrame = true;

    bird.playFlapSound();

    obstacle.initializeObstacle();
    obstacle2.initializeObstacle(1800.f);

    if (paused) {
        pause();
        pauseTime = sf::Time();
    }

} // end initialize()

void HappyBird::flap(sf::Time flapTime) {

    bird.lowerWing();

    if (getFlapTime() < getTotalFlapTime() &&
        (firstFrame || getAnimationTime() > getTotalAnimationTime())) {
        animationClock.restart();
    } else if (flapTime > getTotalFlapTime()) {
        bird.raiseWing();
        setFlapping(false);
    }

    firstFrame = false;

} // end flap()

void HappyBird::dontFlap() {
    bird.raiseWing();
}

bool HappyBird::getFlapping() {
    return flapping;
}

void HappyBird::setFlapping(bool flapping) {
    this->flapping = flapping;
}

sf::Time HappyBird::getFlapTime() {
    return flapTime;
}

sf::Time HappyBird::getAnimationTime() {
    return animationTime;
}

void HappyBird::setFlapTime(sf::Time flapTime) {
    this->flapTime = flapTime;
}

void HappyBird::setAnimationTime(sf::Time animationTime) {
    this->animationTime = animationTime;
}

sf::Time HappyBird::getTotalFlapTime() {
    return totalFlapTime;
}

sf::Time HappyBird::getTotalAnimationTime() {
    return totalAnimationTime;
}

std::string HappyBird::intToString(int num) {
    std::ostringstream convert;
    convert << num;
    return convert.str();
}

Bird HappyBird::getBird() {
    return bird;
}

void HappyBird::killBird() {

    if (!paused) pause();

    bird.setJustDied(true);
    bird.die();

    if (dying) {
        dying    = false;
        birdDied = true;
        --lives;
    }

} // end killBird()

void HappyBird::revive() {

    if (paused) pause();

    birdDied = false;
    dying    = true;

    bird.reinitialize();

    initialize();

} // end revive()

void HappyBird::pause() {

    if (!paused) {
        pauseClock.restart();
        paused = true;
    } else if (!bird.getIsDead()) {
        paused     = false;
        startMusic = true;
    }

} // end pause()

void HappyBird::restart() {

    score         = 0;
    lives         = 3;
    obstacleCount = 0;

    revive();

    obstacle.initializeObstacle();
    obstacle2.initializeObstacle(1800.f);

    initialize();

} // end restart()

void HappyBird::gameOver() {

    if (!paused) pause();
    gameIsOver = true;
    background->setTexture(gameOverTexture);

} // end gameOver()

int HappyBird::getLives() {
    return lives;
}
