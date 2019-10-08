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
#include <Bengine/Errors.h>
#include <iostream>
#include <random>
#include <ctime>


#include "Gun.h"
#include "Level.h"
#include "Player.h"
#include "Zombie.h"

enum class GAME_STATE { PLAY, EXIT };

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.5f;

class MainGame
{
public:
	MainGame();
	~MainGame();

	//runs the entire game
	void run();


private:
	//initialize game systems
	void initSystems();

	//initialize game levels
	void initLevel();

	//initialize shaders to be used
	void initShaders();

	//loop to run while game state is active
	void gameLoop();

	//update game agents
	void updateAgents();

	//update bullets
	void updateBullets();

	//draws the graphics to the screen
	void drawGame();

	//process inputs from the player
	void processInput();

	//check for winning conditions(all zombies dead)
	void checkWin();

	Bengine::Window _window;
	int _screenWidth;
	int _screenHeight;

	GAME_STATE _gameState;

	Bengine::GLSLProgram _colorProgram;

	Bengine::Camera2D _camera;

	Bengine::SpriteBatch _spriteBatch;

	Bengine::InputManager _inputManager;

	Bengine::FpsLimiter _fpsLimiter;

	float _time;
	float _fps;
	float _maxFPS;

	int _currentLevel;

	//vector for storing multiple levels
	std::vector<Level*> _levels;

	//vector for storing all humans
	std::vector<Human*> _humans;

	//vector for storing all Zombies
	std::vector<Zombie*> _zombies;

	Player* _player;

	std::vector<Bullet> _bullets;

	//humans and zombies killed by player
	int _numHumansKilled;
	int _numZombiesKilled;
};