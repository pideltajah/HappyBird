// Bird.h
// Justin Hampton 4/23/14
// Updated for SFML 3.x

#ifndef BIRD_H
#define BIRD_H

#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

constexpr float DEFAULT_Y    = 350.f;
constexpr float DEFAULT_X    = 400.f;
constexpr float BIRD_WIDTH   = 33.f;
constexpr float BIRD_HEIGHT  = 23.f;
constexpr float DEATH_TIME   = 62.5f; // 1/16 of a second
constexpr float DEATH_SPEED1 = 2.f;
constexpr float DEATH_SPEED2 = 4.f;
constexpr float DEATH_SPEED3 = 6.f;

class Bird {

private:
    sf::Texture wingUpTexture;
    sf::Texture wingDownTexture;

    std::optional<sf::Sprite> bird;

    sf::SoundBuffer squawkSound;
    sf::SoundBuffer flapSound;
    sf::SoundBuffer deathSound;

    std::optional<sf::Sound> squawkSFX;
    std::optional<sf::Sound> flapSFX;
    std::optional<sf::Sound> deathSFX;

    bool wingIsUp;
    bool wingIsDown;

    sf::Clock deathClock;
    sf::Time  deathTimer;

    bool justDied;
    bool isDead;

public:
    Bird();

    const sf::Sprite& getBird() const;

    bool getWingIsUp();
    void raiseWing();

    bool getWingIsDown();
    void lowerWing();

    void dive();
    void stopDive();

    void toggleWingPosition();
    void squawk();
    void playFlapSound();

    void die();
    void reinitialize();

    void setJustDied(bool justDied);
    void setIsDead(bool isDead);

    bool getJustDied();
    bool getIsDead();
};

#endif
