// Bird.cpp
// Justin Hampton April 23, 2014
// Updated for SFML 3.x

#include "Bird.h"
#include "HappyBird.h"
#include <iostream>

// Default constructor
Bird::Bird() {

    wingIsUp   = true;
    wingIsDown = false;

    // Load textures
    if (!wingUpTexture.loadFromFile("wingUp2.bmp")) {
        std::cout << "Error: unable to load wingUpTexture from file" << std::endl;
        exit(1);
    }
    if (!wingDownTexture.loadFromFile("wingDown3.bmp")) {
        std::cout << "Error: unable to load wingDownTexture from file" << std::endl;
        exit(1);
    }

    // Load sounds
    if (!squawkSound.loadFromFile("squawk.wav")) {
        std::cout << "unable to open squawk.wav file" << std::endl;
        exit(1);
    }
    if (!flapSound.loadFromFile("flap.wav")) {
        std::cout << "unable to open flap.wav file" << std::endl;
        exit(1);
    }
    if (!deathSound.loadFromFile("death.wav")) {
        std::cout << "unable to open death.wav file" << std::endl;
        exit(1);
    }

    // Initialize sprite and sounds now that resources are loaded
    bird.emplace(wingUpTexture);
    bird->setPosition({DEFAULT_X, DEFAULT_Y});

    squawkSFX.emplace(squawkSound);
    flapSFX.emplace(flapSound);
    deathSFX.emplace(deathSound);

    // Initialize death timer
    justDied = false;
    isDead   = false;

} // end default constructor

const sf::Sprite& Bird::getBird() const {
    return *bird;
}

bool Bird::getWingIsUp() {
    return wingIsUp;
}

// Set wing to up position (also lowers bird)
void Bird::raiseWing() {
    if (wingIsDown) toggleWingPosition();

    if (bird->getPosition().y < (VIEW_HEIGHT + 100)) {
        bird->move({0.f, 2.f});
    }
}

bool Bird::getWingIsDown() {
    return wingIsDown;
}

// Set wing to down position (also raises bird)
void Bird::lowerWing() {
    if (wingIsUp) toggleWingPosition();

    if (bird->getPosition().y > -100) {
        bird->move({0.f, -3.f});
    }
}

// Rapidly drop the bird
void Bird::dive() {
    if (wingIsDown) toggleWingPosition();

    if (bird->getPosition().y < VIEW_HEIGHT) {
        bird->setRotation(sf::degrees(45.f));
        bird->move({0.f, 3.f});
    }
}

// Return bird from dive
void Bird::stopDive() {
    bird->setRotation(sf::degrees(0.f));
}

// Toggle wing position
void Bird::toggleWingPosition() {
    if (wingIsUp) {
        wingIsUp   = false;
        wingIsDown = true;
        bird->setTexture(wingDownTexture);
    } else {
        wingIsUp   = true;
        wingIsDown = false;
        bird->setTexture(wingUpTexture);
    }
}

void Bird::squawk() {
    squawkSFX->play();
}

void Bird::playFlapSound() {
    flapSFX->play();
}

// Death animation
void Bird::die() {

    if (justDied) {
        justDied = false;
        isDead   = true;
        deathClock.restart();
        squawk();
    }

    deathTimer = deathClock.getElapsedTime();

    if (deathTimer < sf::milliseconds(DEATH_TIME)) {
        bird->setRotation(sf::degrees(-135.f));
        bird->move({-DEATH_SPEED3, -DEATH_SPEED1});
    }
    else if (deathTimer > sf::milliseconds(DEATH_TIME) &&
             deathTimer < sf::milliseconds(2 * DEATH_TIME)) {
        bird->setRotation(sf::degrees(-157.5f));
        bird->move({-DEATH_SPEED2, DEATH_SPEED1});
    }
    else if (deathTimer > sf::milliseconds(2 * DEATH_TIME) &&
             deathTimer < sf::milliseconds(3 * DEATH_TIME)) {
        bird->setRotation(sf::degrees(180.f));
        bird->move({-DEATH_SPEED2, DEATH_SPEED2});
    }
    else if (deathTimer > sf::milliseconds(3 * DEATH_TIME) &&
             deathTimer < sf::milliseconds(4 * DEATH_TIME)) {
        bird->setRotation(sf::degrees(157.5f));
        bird->move({-DEATH_SPEED1, DEATH_SPEED2});
    }
    else if (deathTimer > sf::milliseconds(4 * DEATH_TIME) &&
             deathTimer < sf::milliseconds(5 * DEATH_TIME)) {
        bird->setRotation(sf::degrees(135.f));
        bird->move({-DEATH_SPEED1, DEATH_SPEED3});
    }
    else if (deathTimer > sf::milliseconds(5 * DEATH_TIME) &&
             deathTimer < sf::milliseconds(6 * DEATH_TIME)) {
        bird->setRotation(sf::degrees(112.5f));
        bird->move({-DEATH_SPEED1, DEATH_SPEED3});
    }
    else if (deathTimer > sf::milliseconds(6 * DEATH_TIME) &&
             deathTimer < sf::seconds(3.5f)) {
        if (bird->getRotation() == sf::degrees(112.5f)) {
            deathSFX->play();
        }
        bird->setRotation(sf::degrees(90.f));
        bird->move({0.f, DEATH_SPEED3 + 1.f});
    }
    else if (deathTimer > sf::seconds(3.5f)) {
        bird->setRotation(sf::degrees(0.f));
        isDead = false;
    }

} // end die()

void Bird::reinitialize() {
    wingIsUp   = true;
    wingIsDown = false;
    bird->setTexture(wingUpTexture);
    bird->setPosition({DEFAULT_X, DEFAULT_Y});
    bird->setRotation(sf::degrees(0.f));
    justDied = false;
    isDead   = false;
}

void Bird::setJustDied(bool justDied) {
    this->justDied = justDied;
}

void Bird::setIsDead(bool isDead) {
    this->isDead = isDead;
}

bool Bird::getJustDied() {
    return justDied;
}

bool Bird::getIsDead() {
    return isDead;
}
