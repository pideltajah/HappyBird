//	Bird.cpp
//	Justin Hampton
//	April 23, 2014

#include "Bird.h"
#include "HappyBird.h"
#include <iostream>;

//	Default constructor
Bird::Bird(){

	wingIsUp = true;
	wingIsDown = false;

	//	Load the textures
	if ( !wingUpTexture.loadFromFile("wingUp2.bmp") ){
		std::cout << "Error: unable to load wingUpTexture from file" << std::endl;
		exit(1);
	}
	if ( !wingDownTexture.loadFromFile("wingDown3.bmp") ){
		std::cout << "Error: unable to load wingDownTexture from file" << std::endl;
		exit(1);
	}

	//	Load sounds
	if ( !squawkSound.loadFromFile("squawk.wav") ){
		std::cout << "unable to open squawk.wav file" << std::endl;
		exit(1);
	}//end if ( unable to load from file )

	if ( !flapSound.loadFromFile("flap.wav") ){
		std::cout << "unable to open flap.wav file" << std::endl;
		exit(1);
	}//end if ( unable to load from file )

	if ( !deathSound.loadFromFile("death.wav") ){
		std::cout << "unable to open death.wav file" << std::endl;
		exit(1);
	}//end if ( unable to load from file )

	//	Initialize the bird
	bird.setTexture(wingUpTexture);
	/*bird.setOrigin(16, 12);*/
	bird.setPosition(DEFAULT_X, DEFAULT_Y);

	//	Initialize the death Timer
	deathClock = sf::Clock();
	deathTimer = sf::Time();
	justDied = false;
	isDead = false;

}//end default constructor

//	@return the bird sprite
sf::Sprite Bird::getBird(){

	return bird;
}//end getBird() function

//	@return wingIsUp: bool, true if wing is up
bool Bird::getWingIsUp(){

	return wingIsUp;
}//end getWingIsUp() function

//	Set wing to up position
void Bird::raiseWing(){

	if (wingIsDown) toggleWingPosition();

	// Lower the bird
	if ( bird.getPosition().y < (VIEW_HEIGHT + 100) ){
		bird.move(0.0, 2.0);
	}
}//end raiseWing() function

//	@return wingIsDown: bool, true if wing is down
bool Bird::getWingIsDown(){

	return wingIsDown;
}//end getWingIsDown() function

//	Set wing to down position
void Bird::lowerWing(){

	if (wingIsUp) toggleWingPosition();

	// Move bird up
	if ( bird.getPosition().y > -100 ){
		bird.move(0.0, -3.0);
	}
}//end lowerWing() funciton

//	Rapidly drop the bird
void Bird::dive(){
	
	if (wingIsDown) toggleWingPosition();

	if ( bird.getPosition().y < VIEW_HEIGHT ){
		bird.setRotation(45.f);
		bird.move(0.0, 3.0);
	}
}//end dive() function

//	Return bird from dive
void Bird::stopDive(){

	bird.setRotation(0.f);
}//end stopDive() function

//	Toggle wing position
void Bird::toggleWingPosition(){

	if (wingIsUp){
		wingIsUp = false;
		wingIsDown = true;
		bird.setTexture(wingDownTexture);
	}
	else{
		wingIsUp = true;
		wingIsDown = false;
		bird.setTexture(wingUpTexture);
	} 
}//end toggleWingPosition() function

//	Plays the squawkSFX sound
void Bird::squawk(){

	squawkSFX.setBuffer(squawkSound);
	squawkSFX.play();
}//end squawk() function

//	Plays the flapSFX sound
void Bird::playFlapSound(){

	flapSFX.setBuffer(flapSound);
	flapSFX.play();
}//end flapSound() function
	
//	Returns the squawkSFX sound
sf::Sound Bird::getSquawk(){
	return squawkSFX;
}//end getSquawk() function

//	Animation for dead bird
void Bird::die(){

	if (justDied){
		
		justDied = false;
		isDead = true;
		deathClock.restart();
		squawk();
	}

	deathTimer = deathClock.getElapsedTime();

	//	Rotation Guide #1
	if (deathTimer < sf::Time(sf::milliseconds(DEATH_TIME))){

		bird.setRotation(-135.f);
		bird.move(-DEATH_SPEED3, -DEATH_SPEED1);
	}

	//	Rotation Guide #2
	else if ( deathTimer > sf::Time( sf::milliseconds(DEATH_TIME) ) && deathTimer < sf::Time( sf::milliseconds(2 * DEATH_TIME) ) ){

		bird.setRotation(-157.5f);
		bird.move(-DEATH_SPEED2, DEATH_SPEED1);
	}

	//	Rotation Guide #3
	else if ( deathTimer > sf::Time( sf::milliseconds(2 * DEATH_TIME) ) && deathTimer < sf::Time( sf::milliseconds(3 * DEATH_TIME) ) ){
			
		bird.setRotation(180.f);
		bird.move(-DEATH_SPEED2, DEATH_SPEED2);
	}

	//	Rotation Guide #4
	else if ( deathTimer > sf::Time( sf::milliseconds(3 * DEATH_TIME) ) && deathTimer < sf::Time( sf::milliseconds(4 * DEATH_TIME) ) ){
		
		bird.setRotation(157.5f);
		bird.move(-DEATH_SPEED1, DEATH_SPEED2);
	}

	//	Rotation Guide #5
	else if ( deathTimer > sf::Time( sf::milliseconds(4 * DEATH_TIME) ) && deathTimer < sf::Time( sf::milliseconds(5 * DEATH_TIME) ) ){
		
		bird.setRotation(135.f);
		bird.move(-DEATH_SPEED1, DEATH_SPEED3);
	}

	//	Rotation Guide #6
	else if ( deathTimer > sf::Time( sf::milliseconds(5 * DEATH_TIME) ) && deathTimer < sf::Time( sf::milliseconds(6 * DEATH_TIME) ) ){

		bird.setRotation(112.5f);
		bird.move(-DEATH_SPEED1, DEATH_SPEED3);
	}

	//	Rotation Guide #7
	else if ( deathTimer > sf::Time( sf::milliseconds(6 * DEATH_TIME) ) && deathTimer < sf::Time( sf::seconds(3.5) ) ){
		
		if ( bird.getRotation() == 112.5f ){
			
			deathSFX.setBuffer(deathSound);
			deathSFX.play();
		}

		bird.setRotation(90.f);
		bird.move(0.f, DEATH_SPEED3 + 1);
	}

	//	Rotation Guide #8
	else if ( deathTimer > sf::Time( sf::seconds(3.5f) ) ){

		bird.setRotation(0.f);
		isDead = false;
	}

}//end die() method

//	Used to track if bird just died
void Bird::setJustDied(bool justDied){

	this -> justDied = justDied;
}//end justDied() method

//	Used to track if bird is dead
void Bird::setIsDead(bool isDead){

	this -> isDead = isDead;
}//end setIsDead() method

//	Return justDied
bool Bird::getJustDied(){

	return justDied;
}//end getJustDied() method

//	Return isDead
bool Bird::getIsDead(){

	return isDead;
}//end getIsDead() method

