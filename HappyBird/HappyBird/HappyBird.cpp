//	HappyBird.cpp
//	Justin Hampton	4/23/14
//	Updated:		8/22/14
//	Last updated:	Sept 13, 2014

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//	NEEDS:
//		
//		Maybe: Finish the bonus volcano obstacle which will be a one-up
//
//		Add a high score system that allows user to input their name and
//		saves the high scores to a text file.
//
//		Add to the Obstacle class:
//		the speed of the obstacle for variable speed obstacles
//		
//		Music needs some work on getting it to never stop playing
//		music while game is being played. Probably an issue with
//		the music timer working with the pause time or some such.
//		Only happens when the gameOver() function is called and a
//		new game is started.
//
//		Note: A lot of this has been addressed in a more recent version located
//		on Toshiba, As of 12/1/14 the following has not:
//		Change Lives to Health and display 3 hearts to reresent the number of
//		chances to play;
//		Add game over music and maybe more sounds for other things;
//

#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <sstream>
#include <iostream>
#include "HappyBird.h"
#include <string>

//	Default constructor
HappyBird::HappyBird(){

	bird		= Bird();
	obstacle	= Obstacle();
	obstacle2	= Obstacle("obstacle2.bmp");
	bonus		= Obstacle("volcano.bmp");

	flapping	= false;
	firstFrame	= true;
	birdDied	= false;
	dying		= true;
	paused		= false;
	gameIsOver	= false;
	startMusic	= true;

	flapClock		= sf::Clock();
	animationClock	= sf::Clock();
	pauseClock		= sf::Clock();
	musicClock		= sf::Clock();

	flapTime			= sf::Time();
	animationTime		= sf::Time();
	totalFlapTime		= sf::milliseconds(300);
	totalAnimationTime	= sf::milliseconds(3);
	pauseTime			= sf::Time();
	musicTime			= sf::Time();
	totalMusicTime		= sf::Time(sf::seconds(95));

	backgroundTexture	= sf::Texture();
	gameOverTexture		= sf::Texture();

	if ( !backgroundTexture.loadFromFile("background.bmp") ){
		std::cout << "Error: unable to load background texture" << std::endl;
	}

	if ( !gameOverTexture.loadFromFile("gameOver.bmp") ){
		std::cout << "Error: unable to load gameOver texture" << std::endl;
	}

	background		= sf::Sprite(backgroundTexture);

	score			= 0;
	lives			= 3;
	obstacleCount	= 0;

}//end default constructor

//	Play the game!
void HappyBird::play(){

	//	Clouds and mountains(sf::Sprites for 3D effect)
	sf::Texture cloud1Texture	= sf::Texture();
	sf::Texture cloud2Texture	= sf::Texture();
	sf::Texture cloud3Texture	= sf::Texture();
	sf::Texture mountainTexture = sf::Texture();
	
	if ( !cloud1Texture.loadFromFile("cloud1.bmp") ) std::cout << "Error: unable to load cloud1Texture from file" << std::endl;
	if ( !cloud2Texture.loadFromFile("cloud2.bmp") ) std::cout << "Error: unable to load cloud2Texture from file" << std::endl;
	if ( !cloud3Texture.loadFromFile("cloud3.bmp") ) std::cout << "Error: unable to load cloud3Texture from file" << std::endl;
	if ( !mountainTexture.loadFromFile("mountains.bmp") ) std::cout << "unable to load mountainTexture from file" << std::endl;

	sf::Sprite cloud1		= sf::Sprite(cloud1Texture);
	sf::Sprite cloud2		= sf::Sprite(cloud2Texture);
	sf::Sprite cloud3		= sf::Sprite(cloud3Texture);
	sf::Sprite mountains	= sf::Sprite(mountainTexture);
	sf::Sprite mountains2	= sf::Sprite(mountainTexture);

	cloud1.setPosition( VIEW_WIDTH, 73 );
	cloud2.setPosition( VIEW_WIDTH, 79 );
	cloud3.setPosition( 0.f, 75 );
	mountains.setPosition(0.f, 380);
	mountains2.setPosition(1200, 380);

	//	Variables to simplify comparing bird and obstacle positions
	float birdFront;
	float birdBack;
	float birdHead;
	float birdFoot;

	float obstacleFront;
	float obstacleBack;
	float obstacleTop;
	float obstacleBottom;

	float obstacle2Front;
	float obstacle2Back;
	float obstacle2Top;
	float obstacle2Bottom;

	// Smooth edges of graphics
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	////////////////////////////////////////////////////
	//	Components used for game play...
	//

	//////////////////////////////
	// Font for text display	//
	//////////////////////////////
	sf::Font font;
	
	if (!font.loadFromFile("Bicubik.OTF")){
		// error...
		std::cout << "unable to load font";
	}

	//////////////////////////////////	
	// Texts for display messages	//
	//////////////////////////////////
	sf::Text hud;

	hud.setPosition(VIEW_WIDTH -200, 2);

	hud.setFont(font);
	hud.setCharacterSize(14);
	hud.setColor(sf::Color::Red);
	hud.setString("Score: " + std::to_string(score) + "\nLives: " + std::to_string(lives));
	hud.setStyle(sf::Text::Regular);

	//////////////////////////
	// Sounds for gameplay	//
	//////////////////////////
	
	sf::SoundBuffer scoreSound;    
	sf::SoundBuffer backgroundMusic;

	if ( !scoreSound.loadFromFile("score.wav") ){
		std::cout << "unable to open score.wav file" << std::endl;
		exit(1);
	}//end if ( unable to load from file )
    
	if ( !backgroundMusic.loadFromFile("background.wav") ){
		std::cout << "unable to open background.wav file" << std::endl;
		exit(1);
	}//end if ( unable to load from file )

	sf::Sound scoreSFX(scoreSound);	
	sf::Sound backgroundSFX(backgroundMusic);

	//////////////////////////////
	// Display window			//
	//////////////////////////////
	sf::RenderWindow window( sf::VideoMode( VIEW_WIDTH, VIEW_HEIGHT ), "Happy Bird!", sf::Style::Titlebar | sf::Style::Close );
	window.setVerticalSyncEnabled(true);
	
	/////////////////////////////////////////////////////
	//	Game play...
	//

	//	Initialize the game
	initialize();

    // run the program as long as the window is open
    while ( window.isOpen() )
    {
		// Update timers
		if (paused){
			pauseTime += pauseClock.getElapsedTime();
			backgroundSFX.pause();
		}

		// **************Change to: if (playback time == trackTime) backgroundSFX.play();************
		musicTime = musicClock.getElapsedTime() - pauseTime;
		// Replay music at end of music track
		if (backgroundSFX.getPlayingOffset() >= totalMusicTime){

			startMusic = true;
			musicClock.restart();
		}//end replay music

		// Play music
		if (startMusic){

			backgroundSFX.play();
			startMusic = false;
		}//end play music
		// ******************************************************************************************

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

		// while there are pending events...
		while (window.pollEvent(event)){

			// check the type of the event...
			switch (event.type){

				// window closed
				case sf::Event::Closed:
					exit(0);
					break;

				// mouse button clicked
				case sf::Event::MouseButtonReleased:
					//	Left mouse button clicked: flap the bird
					if ( event.key.code == sf::Mouse::Left ){
						if (!paused){

							//	reset the clocks
							flapClock.restart();						
							animationClock.restart();

							//	start flapping
							setFlapping(true);
							firstFrame = true;

							//	play sound effect
							bird.playFlapSound();

						}//end if not paused
					}//end if (left mouse button clicked)

				case sf::Event::KeyReleased:
					if (event.key.code == sf::Keyboard::P){
						pause();
					}
					else if (event.key.code == sf::Keyboard::R){
						restart();
					}
					break;
					
				default:
					break;
			}//end switch (event type)
		}//end pollEvent loop

		//	Update score
		hud.setString("Score: " + std::to_string(score) + "\nLives: " + std::to_string(lives));

		//	Perform game operations if game is not paused
		if ( !paused ){

			//	Check right mouse button for bird dive
			if ( sf::Mouse::isButtonPressed(sf::Mouse::Right) ){
				// right mouse button is pressed: dive
				bird.dive();
			}
			else{
				bird.stopDive();
			}//end check for bird dive	

			//	Update flapping
			setFlapTime( flapClock.getElapsedTime() );
			setAnimationTime( animationClock.getElapsedTime() );		

			//	Check if flapping is in progress. If so, continue flapping.
			if (flapping){
				flap(flapTime);
			}
			else{
				dontFlap();// used to lower bird back to original altitude
			}

			////	Move the clouds	and mountains	////
			if ( cloud1.getPosition().x > -300 ){
				cloud1.move( -0.5, 0.f );
			}
			else{
				cloud1.setPosition( VIEW_WIDTH, 73 );// 73 is somewhat arbitrary, the y position of the cloud
			}

			if ( cloud2.getPosition().x > -300 ){
				cloud2.move( -1.0, 0.f );
			}
			else{
				cloud2.setPosition( VIEW_WIDTH, 79 );// 79 is somewhat arbitrary, the y position of the cloud
			}

			if ( cloud3.getPosition().x > -300 ){
				cloud3.move( -1.5, 0.f );
			}
			else{
				cloud3.setPosition( VIEW_WIDTH, 150 );
			}

			if ( mountains.getPosition().x > -1200 ){
				mountains.move( -.75, 0.f );
			}
			else{
				mountains.setPosition( 1200.f, 380 );
			}

			if ( mountains2.getPosition().x > -1200 ){
				mountains2.move( -.75, 0.f );
			}
			else{
				mountains2.setPosition( 1200.f, 380 );
			}
			////	End move the clouds and mountains	////

			//	Move the obstacles	
			obstacle.moveObstacle();		
			obstacle2.moveObstacle();

			//	Check if bird hit obstacle
			birdFront = ( bird.getBird().getPosition().x + BIRD_WIDTH );
			birdBack = bird.getBird().getPosition().x;
			birdHead = bird.getBird().getPosition().y;
			birdFoot = ( bird.getBird().getPosition().y + (BIRD_HEIGHT * 0.5) );

			obstacleFront = obstacle.getObstacle().getPosition().x;
			obstacleBack = (obstacle.getObstacle().getPosition().x + OBSTACLE_WIDTH);
			obstacleTop = (obstacle.getObstacle().getPosition().y + 750);//750 is pixels down to top of gap in obstacle.bmp
			obstacleBottom = (obstacle.getObstacle().getPosition().y + 850);//850 is pixels down to bottom of gap in obstacle.bmp

			obstacle2Front = obstacle2.getObstacle().getPosition().x;
			obstacle2Back = (obstacle2.getObstacle().getPosition().x + OBSTACLE_WIDTH);
			obstacle2Top = (obstacle2.getObstacle().getPosition().y + 750);//750 is pixels down to top of gap in obstacle.bmp
			obstacle2Bottom = (obstacle2.getObstacle().getPosition().y + 850);//850 is pixels down to bottom of gap in obstacle.bmp

			//	Kill the bird if obstacle is hit
			if ( birdFront > obstacleFront && birdBack < obstacleBack && ( birdHead < obstacleTop || birdFoot > obstacleBottom ) ){
				killBird();
			}//end if bird hit obstacle

			//	Increment score if successfully passed obstacle
			if (birdBack == obstacleBack && !birdDied){
				scoreSFX.play();
				++score;
			}//end increment score

			//	Kill the bird if obstacle2 is hit
			if ( birdFront > obstacle2Front && birdBack < obstacle2Back && (birdHead < obstacle2Top || birdFoot > obstacle2Bottom ) ){
				killBird();
			}//end if bird hit obstacle			

			//	Increment score if successfully passed obstacle2
			if ( birdBack == obstacle2Back && !birdDied ){
				scoreSFX.play();
				++score;
			}//end increment score

		}//end if not paused	

		//	Play animation if bird is dead
		if ( bird.getIsDead() && !gameIsOver ){

			bird.die();
		}//end play animation if bird is dead

		//	Game over
		else if ( lives < 1 && !bird.getIsDead() ){

			gameOver();
		}//end game over

		//	Bring the bird back to life
		else if ( birdDied && !gameIsOver && !bird.getIsDead() ){
			
			revive();
		}//end bring the bird back to life

		// clear the window ( clearing the window before drawing anything is manditory )
		window.clear(sf::Color::Blue);

		//////////////////////////////////////////////////////////////////////////////
        // draw everything here...
		//////////////////////////////////////////////////////////////////////////////

		//	Draw the background
		window.draw(background);

		if (!gameIsOver){

			//	Draw the mountains
			window.draw(mountains);
			window.draw(mountains2);

			//	Draw the clouds
			window.draw(cloud1);
			window.draw(cloud2);
			window.draw(cloud3);

			//	Draw the bird
			window.draw(bird.getBird());

			// Draw the obsticles		
			window.draw(obstacle.getObstacle());		
			window.draw(obstacle2.getObstacle());
		}

		// Draw messages
		window.draw(hud);

        // end the current frame
        window.display();

    }//end main loop

}//end play() function

//	Initializes the game
void HappyBird::initialize(){

	//	Reset gameIsOver
	gameIsOver = false;
	startMusic = true;

	//	Set background
	background.setTexture(backgroundTexture);

	//	Reset the clocks
	flapClock.restart();
	animationClock.restart();
	musicClock.restart();

	//	Start flapping
	setFlapping(true);
	firstFrame = true;

	//	Play sound effect
	bird.playFlapSound();
	
	//	Initialize obstacles
	obstacle.initializeObstacle();
	obstacle2.initializeObstacle(1800);

	//	Reset the pause timer
	if (paused){
		pause();
		pauseTime = sf::Time();
	}
	
}//end initialize() function

//	Flap the birds wing
void HappyBird::flap(sf::Time flapTime){

	//	wing action
	bird.lowerWing();						

	//	elevate the bird	
	if (getFlapTime() < getTotalFlapTime() && (firstFrame || getAnimationTime() > getTotalAnimationTime())){
		animationClock.restart();
	}//end elevating the bird

	else if (flapTime > getTotalFlapTime()){
		bird.raiseWing();
		setFlapping(false);
	}

	firstFrame = false;
}//end flap() function

//	Don't flap the bird's wing. (used to make bird fall)
void HappyBird::dontFlap(){
	bird.raiseWing();
}//end dontFlap() function

//	Get flapping
bool HappyBird::getFlapping(){

	return flapping;
}//end getFlapping() function

//	Set flapping
void HappyBird::setFlapping(bool flapping){

	this -> flapping = flapping;
}//end setFlapping() function

//	Get flapTime
sf::Time HappyBird::getFlapTime(){

	return flapTime;
}//end getFlapTime() function

//	Get animationTime
sf::Time HappyBird::getAnimationTime(){

	return animationTime;
}//end getAnimationTime() function

//	Set flapTime
void HappyBird::setFlapTime(sf::Time flapTime){

	this -> flapTime = flapTime;
}//end setFlapTime() function

//	Set animationTime
void HappyBird::setAnimationTime(sf::Time animationTime){

	this -> animationTime = animationTime;
}//end setAnimationTime() function

//	Get totalFlapTime
sf::Time HappyBird::getTotalFlapTime(){

	return totalFlapTime;
}//end getTotalFlapTime() function

//	Get totalAnimationTime
sf::Time HappyBird::getTotalAnimationTime(){

	return totalAnimationTime;
}//end getTotalAnimationTime() function

//	Converts an int to a string
std::string HappyBird::intToString(int num){
	
	//string stream for conversion
	std::ostringstream convert;

	convert << num;

	return convert.str();

}//end intToString() function

//	Returns the Bird object
Bird HappyBird::getBird(){
	return bird;
}//end getBird() function

//	Kills the bird
void HappyBird::killBird(){

	if ( !paused ) pause();

	bird.setJustDied(true);
	
	bird.die();

	if (dying){
	
		dying = false;
		birdDied = true;
		--lives;
	}

}//end killBird() function

//	Bring bird back to life
void HappyBird::revive(){

	//	Unpause the game
	if (paused) pause();

	//	Reset variables
	birdDied = false;
	dying = true;

	//	Reset the bird and obstacles
	bird = Bird();

	initialize();
	//obstacle = Obstacle(); //done in initialize()
	//obstacle2 = Obstacle("obstacle2.bmp", 1800); //done in initialize()

}//end revive() function

//	Pauses the game
void HappyBird::pause(){
	
	if ( !paused ){
		pauseClock.restart();
		paused = true;
	}
	else if ( !bird.getIsDead() ){
		//pauseTime += pauseClock.getElapsedTime();
		paused = false;
		startMusic = true;
	}
}//end pause() function

//	Starts a new game
void HappyBird::restart(){	
	
	//	reset score and lives
	score = 0;
	lives = 3;
	obstacleCount = 0;

	//	Reset the bird
	revive();

	//	Reset the obstacles
	obstacle = Obstacle();
	obstacle2 = Obstacle("obstacle2.bmp", 1800);

	//	Initialize the new game
	initialize();

}//end restart() function

//	End of the game (no lives remaining)
void HappyBird::gameOver(){
	
	if ( !paused ) pause();
	gameIsOver = true;
	background.setTexture(gameOverTexture);

}//end gameOver function

//	Returns the number of lives
int HappyBird::getLives(){

	return lives;
}//end getLives() method