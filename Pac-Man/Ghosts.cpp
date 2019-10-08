#include "Ghosts.h"

#include <iostream>
#include <math.h>
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Ghosts::Ghosts(glm::vec2 pos, int ghostNum, int speed, glm::vec2 leftTeleport, glm::vec2 rightTeleport) {
	_startLoc = pos;
	_speed = speed;
	
	//set initial position to start position when creating ghosts
	_position = _startLoc;
	_direction = glm::normalize(_position + glm::vec2(100.0f, 100.0f));
	_leftTeleport = leftTeleport;
	_rightTeleport = rightTeleport;

	_ghostNum = ghostNum;

	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	_frameCounter = 0;

}

Ghosts::~Ghosts() {

}

void Ghosts::update(glm::vec2 playerPos, const std::vector<std::string>& levelData) {
	//TO-DO --- Update Ghost AI

	//random number generator for movement when not near player
	static bool seeded = false;
	static std::mt19937 randomEngine;

	//prevent reseeding random engine each init call
	if (!seeded) {
		randomEngine.seed(time(nullptr));
		seeded = true;
	}

	//random number for direction for ghosts
	static std::uniform_real_distribution<float> randRotate(-0.25f, 0.25f);

	
	//get distance to player
	float playerDistance = std::sqrt(((playerPos.x - _position.x) * (playerPos.x - _position.x)) +
		((playerPos.y - _position.y) * (playerPos.y - _position.y)));

	if (playerDistance < MAX_PLAYER_DISTANCE) {

		//direct ghost toward player
		_direction = glm::normalize(playerPos - _position);	
	}
	//move ghosts at random
	else {
		//rotate ghosts to move in random direction
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}

	//if colliding with wall attempt to change direction
	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}

	_position += _direction * _speed;

	//check if at teleport and move ghost if so
	if (_position.x < _leftTeleport.x) {
		_position = _rightTeleport;
	}
	else if (_position.x > _rightTeleport.x) {
		_position = _leftTeleport;
	}
}

void Ghosts::draw(Bengine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	//ghost numbers coorilate to different color ghosts
	//TO-DO implement frameCounter to animate ghosts
	if (_ghostNum == 1) {
		_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_19.png").id;
	}
	else if (_ghostNum == 2) {
		_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_23.png").id;
	}
	else if (_ghostNum == 3) {
		_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_27.png").id;
	}
	else if (_ghostNum == 4) {
		_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_36.png").id;
	}
	else {
		_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_44.png").id;
	}


	_frameCounter++;

	spriteBatch.draw(destRect, uvRect, _textureID, 0.0f, _color);
}
