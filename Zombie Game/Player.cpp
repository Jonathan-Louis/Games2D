#include "Player.h"
#include "Gun.h"

#include <iostream>

Player::Player() :
_currentGun(-1)
{
}

Player::~Player() {}

void Player::init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets) {
	
	_speed = speed;
	_position = pos;

	//player health
	_health = 20;

	_inputManager = inputManager;
	_color.r = 0;
	_color.g = 0;
	_color.b = 200;
	_color.a = 255;
	_camera = camera,
	_bullets = bullets;
}

void Player::addGun(Gun* gun) {
	//add guns
	_guns.push_back(gun);

	//equip first gun once added
	if (_currentGun == -1) {
		_currentGun = 0;
	}
}


void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies) {
	
	//player movement
	if (_inputManager->isKeyPressed(SDLK_w)) {
		_position.y += _speed;
	}
	else if (_inputManager->isKeyPressed(SDLK_s)) {
		_position.y -= _speed;
	}
	
	if (_inputManager->isKeyPressed(SDLK_a)) {
		_position.x -= _speed;
	}
	else if (_inputManager->isKeyPressed(SDLK_d)) {
		_position.x += _speed;
	}


	//selection of guns using 1 2 3
	if (_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0) {
		_currentGun = 0;
	}
	else if (_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 1) {
		_currentGun = 1;
	}
	else if (_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 2) {
		_currentGun = 2;
	}

	glm::vec2 mouseCoords = _inputManager->getMouseCoords();

	mouseCoords = _camera->convertScreenToWorld(mouseCoords);

	glm::vec2 centerPos = _position + glm::vec2(AGENT_RADIUS);

	glm::vec2 direction = glm::normalize(mouseCoords - centerPos);
	
	if (_currentGun != -1) {
		_guns[_currentGun]->update(_inputManager->isKeyPressed(SDL_BUTTON_LEFT), direction, centerPos, *_bullets);
		
	}

	//check level collision
	collideWithLevel(levelData);
}