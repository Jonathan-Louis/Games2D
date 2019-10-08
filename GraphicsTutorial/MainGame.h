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

#include "Bullet.h"

//gameState class to control states of play in game
enum class GameState { PLAY, EXIT };


class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

	

private:
	void initSystems();
	void initShaders();
	void processInput();
	void gameLoop();
	void drawGame();

	Bengine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	Bengine::GLSLProgram _colorProgram;
	
	Bengine::Camera2D _camera;

	Bengine::SpriteBatch _spriteBatch;

	Bengine::InputManager _inputManager;

	Bengine::FpsLimiter _fpsLimiter;

	std::vector<Bullet> _bullets;

	float _time;
	float _fps;
	float _maxFPS;
};

