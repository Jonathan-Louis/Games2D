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
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}

//deconstructor
MainGame::~MainGame() {

}

void MainGame::run() {
	//initialize game systems
	initSystems();

	

	//loop while game active
	gameLoop();
}

//game loop to run game
void MainGame::gameLoop() {

	while (_gameState != GameState::EXIT) {

		_fpsLimiter.begin();

		processInput();
		_time += 0.1;

		_camera.update();

		for (int i = 0; i < _bullets.size();) {

			//updating and deleteing bullets if lifetime is over
			if (_bullets[i].update() == true) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			//progress through bullets if one was not removed to avoid skipping bullets in vector
			else {
				i++;
			}
		}

		drawGame();
		
		_fps = _fpsLimiter.end();
		
		//print every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
		
	}
}

//process inputs from the mouse and keyboard
void MainGame::processInput() {

	//checking for event from keyboard or mouse
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)) {

		switch(evnt.type) {
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

	if (_inputManager.isKeyDown(SDLK_w)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_s)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_d)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0));
	}
	if (_inputManager.isKeyDown(SDLK_a)) {
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0));
	}
	if (_inputManager.isKeyDown(SDLK_q)) {
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);

		glm::vec2 playerPosition(0.0f, 0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 5.0f, 1000);
	}

}
	


//initialize systems
void MainGame::initSystems() {

	Bengine::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	//intializes shaders
	initShaders();

	//initialize spritebatch
	_spriteBatch.init();

	_fpsLimiter.init(_maxFPS);
}

//create shader program
void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert.txt", "Shaders/colorShading.frag.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

//draws the game
void MainGame::drawGame() {


	//sets the base depth to 1.0
	glClearDepth(1.0);
	//clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//use shader program
	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);

	//setting uniform variables for shader
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);


	//set the camera matrix
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	
	//draw sprites using sprite batch
	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Bengine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unUse();

	//swap buffer and draw to screen
	_window.swapBuffer();
}