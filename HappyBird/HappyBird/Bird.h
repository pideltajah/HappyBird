//	Bird.h
//	Justin Hampton
//	4/23/14

#ifndef BIRD_H
#define BIRD_H

#define DEFAULT_Y 350
#define DEFAULT_X 400
#define BIRD_WIDTH 33.f
#define BIRD_HEIGHT 23.f
#define DEATH_TIME 62.5F// 1/16 of a second
#define DEATH_SPEED1 2.f
#define DEATH_SPEED2 4.f
#define DEATH_SPEED3 6.f

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

class Bird{

private:

	sf::Texture wingUpTexture;
	sf::Texture wingDownTexture;

	sf::Sprite bird;

	sf::SoundBuffer squawkSound;
	sf::SoundBuffer flapSound;
	sf::SoundBuffer deathSound;

	sf::Sound squawkSFX;
	sf::Sound flapSFX;
	sf::Sound deathSFX;

	//	Toggle switch, only one wing position value
	//	can be true at any given time. If one is set
	//	to true, the other will automatically be set
	//	to false.
	bool wingIsUp;
	bool wingIsDown;

	//	Timer for dying bird animation
	sf::Clock deathClock;
	sf::Time deathTimer;

	bool justDied;
	bool isDead;

public:

	Bird();

	sf::Sprite getBird();
	
	bool getWingIsUp();
	void raiseWing();

	bool getWingIsDown();
	void lowerWing();

	void dive();
	void stopDive();

	void toggleWingPosition();
	void squawk();
	void playFlapSound();

	sf::Sound getSquawk();
	
	void die();

	void setJustDied(bool justDied);
	void setIsDead(bool isDead);

	bool getJustDied();
	bool getIsDead();

};//end class

#endif