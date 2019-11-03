#include "MainGame.h"

#include <Bengine/Errors.h>
#include <Bengine/Sprite.h>
#include <Bengine/ResourceManager.h>
#include <iostream>



//constructor
MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_time(0),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f),
	_numPelletsEaten(0)
{
	_camera.init(_screenWidth, _screenHeight);
	_camera.setScale(2.0f);
}

//deconstructor
MainGame::~MainGame() {

}

void MainGame::run() {
	//initialize game systems
	initSystems();

	//initialize levels
	initLevels();

	//loop while game active
	gameLoop();

	while (_player->getLives() > 0) {

		//initialize levels
		initLevels();

		//loop while game active
		gameLoop();
	}

	std::printf("~~~~GAME OVER~~~~\nTotal Pellets Eaten: %d\n", _numPelletsEaten);
	Bengine::fatalError("");
}

//game loop to run game
void MainGame::gameLoop() {

	while (_gameState != GameState::EXIT) {

		_fpsLimiter.begin();

		processInput();

		//increase time for fps limiter
		_time += 0.1;

		//set camera to follow player
		_camera.setPosition(_player->getPosition());

		//update camera
		_camera.update();

		//update player
		updatePlayer();

		//update ghosts
		updateGhosts();

		//draw the game to the screen
		drawGame();

		_fps = _fpsLimiter.end();

		//print every 10000 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}

		//check for player out of lives
		if (checkGameOver()) {
			break;
		}

		//check if all pellets eaten
		if (checkLevelEnd()) {
			break;
		}

	}

	//destroy current remaining ghosts
	deleteGhosts();
}

//process inputs from the mouse and keyboard
void MainGame::processInput() {

	//checking for event from keyboard or mouse
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {

		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;


		}
	}

	
	if (_inputManager.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	

}



//initialize systems
void MainGame::initSystems() {

	Bengine::init();

	_window.create("*****   PAC-MAN   *****", _screenWidth, _screenHeight, 0);

	//intializes shaders
	initShaders();

	//initialize spritebatch
	_spriteBatch.init();

	_fpsLimiter.init(_maxFPS);
}


void MainGame::initLevels() {

	//input level data
	_levels.emplace_back(new Level("Levels/Level2.txt"));
	_currentLevel = 0;
	for (int i = 0; i < _levels[_currentLevel]->getNumPellets(); i++) {
		_pellets.emplace_back(new Pellets());
		_pellets[i]->setPos(_levels[_currentLevel]->getPelletsLoc(i));
	}
	for (int i = 0; i < _levels[_currentLevel]->getNumBigPellets(); i++) {
		_bigPellets.emplace_back(new Pellets());
		_bigPellets[i]->setPos(_levels[_currentLevel]->getBigPelletsLoc(i));
		_bigPellets[i]->setTexture();
	}

	_player = new Player;
	_player->init(PLAYER_SPEED, &_inputManager, &_camera, _levels[_currentLevel]->getPlayerStartPos(), _levels[_currentLevel]->getLeftTeleport(), _levels[_currentLevel]->getRightTeleport());

	for (int i = 0; i < 4; i++) {
		_ghosts.emplace_back(new Ghosts(_levels[_currentLevel]->getGhostStart(), i + 1, GHOST_SPEED, _levels[_currentLevel]->getLeftTeleport(), _levels[_currentLevel]->getRightTeleport()));
	}
}


//create shader program
void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert.txt", "Shaders/colorShading.frag.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}


void MainGame::updatePlayer() {

	_player->update(_levels[_currentLevel]->getLevelData());

	//check collision for player
	for (int i = 0; i < _pellets.size(); i++) {
		if (_player->collideWithPellets(_pellets[i])) {
			//remove pellet if eaten
			_pellets[i] = _pellets.back();
			_pellets.pop_back();
			_numPelletsEaten++;
		}
		if (i < _bigPellets.size()) {
			if (_player->collideWithBigPellets(_bigPellets[i])) {
				//remove pellet if eaten
				_bigPellets[i] = _bigPellets.back();
				_bigPellets.pop_back();
				_numPelletsEaten++;
			}
		}
	}

	//check for collision with ghosts
	for (int i = 0; i < _ghosts.size(); i++) {
		if (_player->collideWithAgents(_ghosts[i])) {
			_player->removeLife();
			_player->setPosition(_levels[_currentLevel]->getPlayerStartPos());
		}
	}
}

void MainGame::updateGhosts() {

	for (int i = 0; i < _ghosts.size(); i++) {
		_ghosts[i]->update(_player->getPosition(), _levels[_currentLevel]->getLevelData());
	}

}

void MainGame::deleteGhosts() {
	int ghostSize = _ghosts.size();
	for (int i = 0; i < ghostSize; i++) {
		_ghosts.pop_back();
	}
}


//draws the game
void MainGame::drawGame() {


	//sets the base depth to 1.0
	glClearDepth(1.0);
	//clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);

	//use shader program
	_colorProgram.use();

	//prepare to use textures
	glActiveTexture(GL_TEXTURE0);

	//setting uniform variables for shader
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);


	//set the camera matrix
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//draw level
	_levels[_currentLevel]->draw();

	//draw sprites using sprite batch
	_spriteBatch.begin();

	//draw the player
	_player->draw(_spriteBatch);

	//draw the pellets
	for (int i = 0; i < _pellets.size(); i++) {
		_pellets[i]->draw(_spriteBatch, _pellets[i]->getDestRect());
	}

	//draw the big pellets
	for (int i = 0; i < _bigPellets.size(); i++) {
		_bigPellets[i]->draw(_spriteBatch, _bigPellets[i]->getDestRect());
	}

	//draw the ghosts
	for (int i = 0; i < _ghosts.size(); i++) {
		_ghosts[i]->draw(_spriteBatch);
	}
	
	//close the spritebatch
	_spriteBatch.end();

	//render current spritebatch
	_spriteBatch.renderBatch();

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//close shader program
	_colorProgram.unUse();

	//swap buffer and draw to screen
	_window.swapBuffer();
}


bool MainGame::checkLevelEnd() {
	if (_pellets.size() == 0) {
		return true;
	}

	return false;
}


bool MainGame::checkGameOver() {
	if (_player->getLives() == 0) {
		return true;
	}

	return false;
}