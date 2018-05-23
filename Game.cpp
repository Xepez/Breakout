#include "Game.h"

//Delete this
#include <iostream>
using namespace std;

Game::Game(int width, int height) {
    //Constructor

    //Platform
    plat = new Platform("images/plat.jpg", -0.125, -0.81, 0.05, 0.25);
    //Initial x, y coords for ball
    Vec center((plat->getX() + (plat->getW()/2)), (plat->getY() + 0.025 + 0.0001));
    //Ball
    ball = new Ball("images/ball.png", center, 0.025, 10);

    // levels
    ///brick = new Brick("images/block.png", 0, 0, .5, .5, 1, 1, 0);
    GameLevel one; one.load("levels/one.lvl", width, height * 0.5);
    GameLevel two; two.load("levels/two.lvl", width, height * 0.5);
    GameLevel three; three.load("levels/three.lvl", width, height * 0.5);
    GameLevel four; four.load("levels/four.lvl", width, height * 0.5);
    levels.push_back(one);
    levels.push_back(two);
    levels.push_back(three);
    levels.push_back(four);
    level = 0;

    this->width = width;
    this->height = height;

    ballSpeed = 0.005;
    platSpeed = 0.02;
    resetMark = true;	

    lives = 3;
    setLivesDisp("images/lives3.png");

    end = pause = false;
    setEndDisp("images/end.png");

    setBackDisp("images/factory.jpg");
}

Game::Game(const char* platPic, const char* ballPic, const char* backgroundPic, int width, int height) {
	//Constructor

    //Platform
    plat = new Platform(platPic, -0.125, -0.81, 0.05, 0.25);
	//Initial x, y coords for ball
	Vec center((plat->getX() + (plat->getW()/2)), (plat->getY() + 0.025 + 0.0001));
	//Ball
	ball = new Ball(ballPic, center, 0.025, 10);

    // levels
    ///brick = new Brick("images/block.png", 0, 0, .5, .5, 1, 1, 0);
    GameLevel one; one.load("levels/one.lvl", width, height * 0.5);
    GameLevel two; two.load("levels/two.lvl", width, height * 0.5);
    GameLevel three; three.load("levels/three.lvl", width, height * 0.5);
    GameLevel four; four.load("levels/four.lvl", width, height * 0.5);
    levels.push_back(one);
    levels.push_back(two);
    levels.push_back(three);
    levels.push_back(four);
    level = 0;

    this->width = width;
    this->height = height;

	ballSpeed = 0.005;
	platSpeed = 0.02;
	resetMark = true;	

	lives = 3;
	setLivesDisp("images/lives3.png");

	end = pause = false;
	setEndDisp("images/end.png");

	setBackDisp(backgroundPic);
}

Game::~Game() { 
	//Destructor
	delete plat;
	delete ball;
}


bool Game::isWinner() {

	for (int i = 0; i < levels[level].bricks.size(); i++) {
        if (!levels[level].bricks[i].isDestroyed() && !levels[level].bricks[i].isSolid()) {
			return false;
		}
	}

	resetLives();
	reset();
	level++;

	return true;
}

void Game::setBackDisp(const char* bgPic) {
	//Sets background image
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    
    background_id = SOIL_load_OGL_texture (
    	bgPic,
    	SOIL_LOAD_AUTO,
    	SOIL_CREATE_NEW_ID,
     	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void Game::displayBackground() {
	//Displays background image
	glBindTexture( GL_TEXTURE_2D, background_id );
    glEnable(GL_TEXTURE_2D);
    
    glBegin(GL_QUADS);
    glColor4f(1, 1, 1, 1);
    glTexCoord2f(0, 0);
    glVertex2f(-1.0, -1.0);
    
    glTexCoord2f(0, 1);
    glVertex2f(-1.0, 1.0);
    
    glTexCoord2f(1, 1);
    glVertex2f(1.0, 1.0);
    
    glTexCoord2f(1, 0);
    glVertex2f(1.0, -1.0);
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void Game::resetLives() {
	lives = 3;
	setLivesDisp("images/lives3.png");
}

void Game::setLivesDisp(const char* livesPic) {
	//Sets lives image
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    
    lives_id = SOIL_load_OGL_texture (
    	livesPic,
    	SOIL_LOAD_AUTO,
    	SOIL_CREATE_NEW_ID,
     	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void Game::displayLives() {
	//Displays lives image
	glBindTexture( GL_TEXTURE_2D, lives_id );
    glEnable(GL_TEXTURE_2D);
    
    glBegin(GL_QUADS);
    glColor4f(1, 1, 1, 1);
    glTexCoord2f(0, 0);
    glVertex2f(0.7, 0.7);
    
    glTexCoord2f(0, 1);
    glVertex2f(0.7, 1.0);
    
    glTexCoord2f(1, 1);
    glVertex2f(1.0, 1.0);
    
    glTexCoord2f(1, 0);
    glVertex2f(1.0, 0.7);
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void Game::draw() {
	//Main draw fcn
	displayBackground();

	plat->print();
	ball->print();
	levels[level].draw();

	displayLives();

	if (end)
		displayEnd();
}

void Game::movePlatform(float m) {
	//moves the plat if game is still in progress
	if (!end && !pause) {
		plat->move(m);
		if (resetMark == true) {
			ball->setX(plat->getX() + (plat->getW()/2));
			ball->setY(plat->getY() + ball->getRadius() + 0.0001);
			ball->print();
		}
	}
}

void Game::moveBall() {
	//Moves the ball if game is still in progress
	if (!end && !pause)
		ball->jump();
}

void Game::collision(){
	//Collision of the ball with the platforms and blocks
	float mx = ball->getX();
	float my = ball->getY();
	float rad = ball->getRadius();

	if (plat->contains((mx - rad), (my - rad)) || plat->contains((mx + rad), (my - rad))) {
		//Ball hits platform

		if (ball->getXinc() == 0.0)	//Incase of reset allows ball to move in x direction
			ball->setXinc(0.001);

		ball->setRising(true);

		if (mx > plat->getX() && mx <= (plat->getX() + (plat->getW() / 2))) {
			if (ball->getMoveLeft())
				ball->setXinc(ballSpeed);

			if (!(ball->getMoveLeft()))
				ball->setXinc(-1 * ballSpeed);
		}
		else if (mx > (plat->getX() + (plat->getW() / 2)) && mx <= (plat->getX() + plat->getW())){
			if (ball->getMoveLeft())
				ball->setXinc(-1 * ballSpeed);

			if (!(ball->getMoveLeft()))
				ball->setXinc(ballSpeed);
		}

	}
	
	for (int i = 0; i < levels[level].bricks.size(); i++) {
            if (!levels[level].bricks[i].isDestroyed()) {
				// If collision is true
            	float tempX, tempY;
            	if (levels[level].bricks[i].contains(mx, my, rad, tempX, tempY)) {
                    if (!levels[level].bricks[i].isSolid()) {
                        levels[level].bricks[i].setDestroyed(true);
                        isWinner();
                    	//spawnPowerUps(box);
                    }

                    if (ball->getX() <= levels[level].bricks[i].getX())
                    	ball->setMoveLeft(true);
                    if (ball->getX() > levels[level].bricks[i].getX() + levels[level].bricks[i].getW())
                    	ball->setMoveLeft(false);
                    if (ball->getY() <= levels[level].bricks[i].getY() - levels[level].bricks[i].getH())
                    	ball->setRising(false);
                    if (ball->getY() > levels[level].bricks[i].getY())
                    	ball->setRising(true);
			}      
        }
    }

	if (my < (plat->getY() - plat->getH())) {
		reset();	//Resets if ball goes under platform
		--lives;

		if (lives == 2)
			setLivesDisp("images/lives2.png");
		if (lives == 1)
			setLivesDisp("images/lives1.png");
		if (lives == 0)
			end = true;
	}
}

void Game::reset() {
	//Resets the game after each life is lost
	ball->setX(plat->getX() + (plat->getW()/2));
	ball->setY(plat->getY() + ball->getRadius() + 0.0001);
	ball->stop();
	ball->print();
	resetMark = true;
}

void Game::resetGame() {
	if (level == 0)levels[0].load("levels/one.lvl", this->width, this->height * 0.5f);
    else if (level == 1)levels[1].load("levels/two.lvl", this->width, this->height * 0.5f);
    else if (level == 2)levels[2].load("levels/three.lvl", this->width, this->height * 0.5f);
    else if (level == 3)levels[3].load("levels/four.lvl", this->width, this->height * 0.5f);

    resetLives();
	reset();
	end = false;
}

void Game::setEndDisp(const char* endPic) {
	//Sets end game image
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    
    end_id = SOIL_load_OGL_texture (
    	endPic,
    	SOIL_LOAD_AUTO,
    	SOIL_CREATE_NEW_ID,
     	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void Game::displayEnd() {
	//Displays end game image
	glBindTexture( GL_TEXTURE_2D, end_id );
    glEnable(GL_TEXTURE_2D);
    
    glBegin(GL_QUADS);
    glColor4f(1, 1, 1, 1);
    glTexCoord2f(0, 0);
    glVertex2f(-0.2, -0.2);
    
    glTexCoord2f(0, 1);
    glVertex2f(-0.2, 0.2);
    
    glTexCoord2f(1, 1);
    glVertex2f(0.2, 0.2);
    
    glTexCoord2f(1, 0);
    glVertex2f(0.2, -0.2);
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void Game::functionKey(unsigned char key) {
	if (key == 78) {	// N
	   resetGame();
    }

    if (key == 80) {	// P
        pausing();
    }

    if (key == 91) {	// [
        decLevel();
		glutPostRedisplay();
    } 
    if (key == 93) {	// ]
		incLevel();
		glutPostRedisplay();
    }
}


void Game::pausing() {
	pause = !pause;
}

void Game::decLevel() {
	if ((level - 1) == -1)
		level = (levels.size() - 1);
	else
		--level;
}

void Game::incLevel() {
	if ((level + 1) == levels.size())
		level = 0;
	else
		++level;
}

bool Game::newResetMark() {
	if (resetMark) {
		ball->setYinc(2 * ballSpeed);
		resetMark = false;
		return true;
	}
	return false;
}

void Game::setBallSpeed(float ballSpeed) {
	this->ballSpeed = ballSpeed;
}
void Game::setPlatSpeed(float platSpeed) {
	this->platSpeed = platSpeed;
}

float Game::getBallSpeed() const {
	return ballSpeed;
}
float Game::getPlatSpeed() const {
	return platSpeed;
}

