//	Obstacle.h
//	Justin Hampton July 9, 2014

#ifndef OBSTACLE_H
#define OBSTACLE_H

#define OBSTACLE_WIDTH 100.f
#define OBSTACLE_HEIGHT 1600.f
#define VIEW_WIDTH 1200.f

#include <SFML\Graphics.hpp>

class Obstacle{

private:
	sf::Texture obstacleTexture;
	sf::Sprite obstacle;

	float randNum;

public:
	Obstacle ( std::string obstacleFile = "obstacle.bmp", float x = VIEW_WIDTH, float y = 0.f );

	void moveObstacle();

	sf::Sprite getObstacle();

	void initializeObstacle( float xPosition = VIEW_WIDTH );

};//end class

#endif