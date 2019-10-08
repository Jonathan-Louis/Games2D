#include "Ghosts.h"

#include <iostream>

Ghosts::Ghosts(glm::vec2 pos, int ghostNum, int speed) {
	_startLoc = pos;
	_speed = speed;
	
	//set initial position to start position when creating ghosts
	_position = _startLoc;

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
	_direction = playerPos - _position;

	_direction = glm::normalize(_direction);

	_position += _direction * _speed;

	collideWithLevel(levelData);
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
