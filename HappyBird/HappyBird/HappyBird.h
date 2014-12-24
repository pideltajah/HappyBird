//	HappyBird.h
//	Justin Hampton
//	4/23/14

#ifndef HAPPYBIRD_H
#define HAPPYBIRD_H

#define VIEW_WIDTH 1200.f
#define VIEW_HEIGHT 800.f

#include "Bird.h"
#include "Obstacle.h"

class HappyBird{

private:

	Bird bird;
	Obstacle obstacle;
	Obstacle obstacle2;
	Obstacle bonus;

	bool flapping;		// This should belong to the Bird class
	bool firstFrame;	// Used for flapping animation (to raise bird over set amount of time)
	bool birdDied;
	bool dying;
	bool paused;
	bool gameIsOver;
	bool startMusic;	// Used to play background music at the start of a new game or on resume

	sf::Clock flapClock;		//	elapsed time since flapping started
	sf::Clock animationClock;	//	elapsed time since last animation update
	sf::Clock pauseClock;		//	Clock to keep up with amount of time game is paused
	sf::Clock musicClock;		//	For tracking time of background music

	sf::Time flapTime;
	sf::Time animationTime;
	sf::Time totalFlapTime;
	sf::Time totalAnimationTime;
	sf::Time pauseTime;
	sf::Time musicTime;
	sf::Time totalMusicTime;

	sf::Texture backgroundTexture;
	sf::Texture gameOverTexture;
	sf::Texture highScoreTexture;

	sf::Sprite background;

	int score;
	int lives;	
	int obstacleCount;

public:

	//	Default constructor
	HappyBird();

	void play();
	void initialize();
	void flap(sf::Time flapTime);
	void dontFlap();// Calls raiseWing() function to lower bird's position

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

	void moveObstacle(sf::Sprite o);

	Bird getBird();

	void killBird();
	void revive();
	void pause();
	void restart();
	void gameOver();

};//end class


#endif

















