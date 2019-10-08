#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <Bengine/Sprite.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>
#include <Bengine/Bengine.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>
#include <Bengine/Timing.h>

#include "Level.h"
#include "Player.h"
#include "Pellets.h"
#include "Ghosts.h"

//gameState class to control states of play in game
enum class GameState { PLAY, EXIT };

const float PLAYER_SPEED = 3.5f;
const float GHOST_SPEED = 2.5f;

class MainGame
{
public:
	MainGame();
	~MainGame();

	//runs the game
	void run();



private:

	//loop to run while game state is active
	void gameLoop();

	//process inputs from the player
	void processInput();

	//initialize game systems
	void initSystems();

	//initialize the level
	void initLevels();

	//initialize shaders to be used
	void initShaders();

	//update the player
	void updatePlayer();

	//update the ghosts
	void updateGhosts();
	
	//draws the graphics to the screen
	void drawGame();

	//check if all pellets eaten
	bool checkLevelEnd();

	//check if player out of lives
	bool checkGameOver();

	Bengine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	Bengine::GLSLProgram _colorProgram;

	Bengine::Camera2D _camera;

	Bengine::SpriteBatch _spriteBatch;

	Bengine::InputManager _inputManager;

	Bengine::FpsLimiter _fpsLimiter;

	float _time;
	float _fps;
	float _maxFPS;

	std::vector<Level*> _levels;

	int _currentLevel;

	Player* _player;

	std::vector<Pellets*> _pellets;

	int _numPelletsEaten;

	std::vector<Ghosts*> _ghosts;
};


