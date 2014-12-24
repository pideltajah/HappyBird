//	Obstacle.cpp
//	Justin Hampton, July 9, 2014

#include "Obstacle.h"
#include "HappyBird.h"
#include <iostream>
#include <time.h>

//	Default constructor
Obstacle::Obstacle( std::string obstacleFile, float x, float y ){

	
	if (!obstacleTexture.loadFromFile(obstacleFile)){
		std::cout << "unable to load obstacleTexture from file" << std::endl;
	}
	
	obstacle.setTexture(obstacleTexture);
	obstacle.setPosition( x, y );

	//	Random number for obstacle position
	srand( time(NULL) );
	randNum = rand() % 800 - VIEW_HEIGHT;

}//end default contructor

//	Moves the obstacle across the screen
void Obstacle::moveObstacle(){

	if ( obstacle.getPosition().x > -300 ){

		obstacle.move( -2.f, 0.f );
	}

	//	Reset obstacle
	else{
		randNum = rand() % 800 - VIEW_HEIGHT;
		obstacle.setPosition( VIEW_WIDTH, randNum );
	}

}//end moveObstacle() function

//	Returns the obstacle sf::sprite
sf::Sprite Obstacle::getObstacle(){

	return obstacle;
}//end getObstacle() function

//	Initializes the obstacle (to avoid white square on start up)
void Obstacle::initializeObstacle( float xPosition ){	

	randNum = rand() % 800 - VIEW_HEIGHT;

	obstacle.setTexture( obstacleTexture );
	obstacle.setPosition( xPosition, randNum );

}//end initializeObstacle() function