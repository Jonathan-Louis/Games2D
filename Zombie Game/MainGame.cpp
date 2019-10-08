#include "MainGame.h"



MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_time(0),
	_gameState(GAME_STATE::PLAY),
	_maxFPS(60.0f),
	_player(nullptr),
	_numHumansKilled(0),
	_numZombiesKilled(0)
{
	_camera.init(_screenWidth, _screenHeight);
}

MainGame::~MainGame() {
	//freeing memory from creating levels
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
}


//runs the entire game
void MainGame::run() {

	//initialize systems
	initSystems();

	//initialize level
	initLevel();

	//gameloop until end of game
	gameLoop();

}

//initialize game systems
void MainGame::initSystems() {

	//initialize game engine
	Bengine::init();

	//create game window
	_window.create("Zombies!", _screenWidth, _screenHeight, 0);

	//initialize shader program
	initShaders();

	//initiate sprite batching
	_spriteBatch.init();

	//enable limiter on FPS
	_fpsLimiter.init(_maxFPS);


	//set camera scale
	_camera.setScale(1.0);

	//update camera with scale
	_camera.update();
	
}

//update if adding additional levels
void MainGame::initLevel() {

	//load level 1 data
	_levels.emplace_back(new Level("Levels/Level1.txt"));
	_currentLevel = 0;

	_player = new Player();
	_player->init(4.0f, _levels[_currentLevel]->getPlayerStartPos(), &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player);

	//random number generator
	static std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randPosX(2, _levels[_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randPosY(2, _levels[_currentLevel]->getHeight() - 2);
	
	//initializing humans
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
		_humans.push_back(new Human());
		glm::vec2 pos(randPosX(randomEngine) * TILE_WIDTH, randPosY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	//initialize zombies
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPos();

	for (int i = 0; i < zombiePositions.size(); i++) {
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	//add players guns
	_player->addGun(new Gun("Revolver", 12, 0.2f, 1, 20.0f, 40));
	_player->addGun(new Gun("Shotgun", 30, 1, 20, 20.0f, 10));
	_player->addGun(new Gun("Machine Gun", 3, 0.5f, 1, 20.0f, 20));
}

//create shader program
void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert.txt", "Shaders/colorShading.frag.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

//loop to run while game state is active
void MainGame::gameLoop() {
	while (_gameState != GAME_STATE::EXIT) {

		_fpsLimiter.begin();

		checkWin();

		processInput();
		
		updateAgents();

		updateBullets();
		
		_time += 0.1;

		_camera.setPosition(_player->getPosition());

		_camera.update();

		drawGame();

		_fps = _fpsLimiter.end();

		//print every 100 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 1000) {
			std::cout << "Current FPS: " << _fps << std::endl;
			frameCounter = 0;
		}

	}
}

void MainGame::updateAgents() {
	//update humans(including player)
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}

	//zombies
	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}

	//update human collisions
	for (int i = 0; i < _humans.size(); i++) {
		//collide with humans
		for (int j = i + 1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}

	//update zombie collisions
	for (int i = 0; i < _zombies.size(); i++) {
		//collide with zombies
		for (int j = i + 1; j < _zombies.size(); j++) {
			_zombies[i]->collideWithAgent(_zombies[j]);
		}

		//collide with humans
		for (int j = 1; j < _humans.size(); j++) {
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				//add new zombie
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				
				//delete human
				delete _humans[j]; ///clear memory of human vector
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}

		//collide with player(trigger gameover)
		if (_zombies[i]->collideWithAgent(_player)) {
			Bengine::fatalError("You Lose");
		}
	}

}

void MainGame::updateBullets() {

	//collide with level
	for (int i = 0; i < _bullets.size();) {
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData())) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	//collide with agents
	for (int i = 0; i < _bullets.size();i++) {

		//avoid looping through humans if bullet used on zombie
		bool bulletRemoved = false;
		
		//loop through zombies
		for (int j = 0; j < _zombies.size();) {

			if (_bullets[i].collideWithAgent(_zombies[j])) {
				
				//damage zombie, if killed remove zombie
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					//clear zombie from memory
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				} else { 
					j++;
				}

				//remove bullet
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				i--; //avoid skipping bullet in bullet<vector> when one removed
				bulletRemoved = true;

				//break the zombie loop if bullet hit a zombie
				break;
			} else { 
				j++;
			}
		}

		//loop through humans
		if (!bulletRemoved) {
			//skip player and start with first human
			for (int j = 1; j < _humans.size();) {
				if (_bullets[i].collideWithAgent(_humans[j])) {

					//damage human, if killed remove zombie
					if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
						//clear human from memory
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_numHumansKilled++;
					}
					else {
						j++;
					}

					//remove bullet
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					i--; //avoid skipping bullet in bullet<vector> when one removed
					//break the zombie loop if bullet hit a zombie
					break;
				}
				else {
					j++;
				}
			}
		}


	}

}

//draws the graphics to the screen
void MainGame::drawGame() {

	//sets the base depth to 1.0
	glClearDepth(1.0);
	//clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0);

	//use shader program
	_colorProgram.use();

	//assigns texture unit 0 for shader
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

	//draw the humans
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->draw(_spriteBatch);
	}

	//draw the zombies
	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->draw(_spriteBatch);
	}

	//draw the bullets
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}

	//close spritebatch and create rendering order
	_spriteBatch.end();

	//render current sprite batch
	_spriteBatch.renderBatch();

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unUse();

	//swap buffer and draw to screen
	_window.swapBuffer();
}


//process inputs from the player
void MainGame::processInput() {
	//checking for event from keyboard or mouse
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {

		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GAME_STATE::EXIT;
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
	
	if (_inputManager.isKeyPressed(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
		std::cout << _camera.getScale() << std::endl;
	}
}

void MainGame::checkWin() {
	if (_zombies.empty()) {
		std::printf("@@@@   You Win!!!!   @@@@\nYou killed %d humans and %d zombies.\nThere are %d/%d humans remaining.\n", _numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		Bengine::fatalError("");
	}
	
}