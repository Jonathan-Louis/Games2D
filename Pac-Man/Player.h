#pragma once

#include <Bengine/InputManager.h>
#include <Bengine/ResourceManager.h>
#include <Bengine/Camera2D.h>
#include <SDL/SDL.h>

#include "Agents.h"

class Player : public Agents
{
public:
	Player();
	~Player();

	//initialize the player
	void init(float speed, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, glm::vec2 pos, glm::vec2 leftTeleport, glm::vec2 rightTeleport);

	void update(const std::vector<std::string>& levelData);

	void draw(Bengine::SpriteBatch& spriteBatch);

	//remove life when contact with ghosts
	void removeLife() { _lives--; }

	//getters
	glm::vec2 getPosition() { return _position; }
	int getLives() { return _lives; }

private:
	Bengine::InputManager* _inputManager;
	Bengine::Camera2D* _camera;
	int _frameCounter;
	int _textureID;
	bool _playerDirection; //true if going right, false if going left
	glm::vec2 _leftTeleport;
	glm::vec2 _rightTeleport;
	int _lives;
	int _direction;	//0 = right, 1 = left, 2 = up, 3 = down
};

