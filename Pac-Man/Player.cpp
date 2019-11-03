#include "Player.h"

#include <iostream>


Player::Player() {

}

Player::~Player() {

}

void Player::init(float speed, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, glm::vec2 pos, glm::vec2 leftTeleport, glm::vec2 rightTeleport) {
	_speed = speed;
	_inputManager = inputManager;
	_camera = camera;
	_position = pos;
	_leftTeleport = leftTeleport;
	_rightTeleport = rightTeleport;

	_lives = 3;
	_playerDirection = true;
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;
	_frameCounter = 0;
	_direction = 0;
}

void Player::update(const std::vector<std::string>& levelData) {

	//player movement
	if (_inputManager->isKeyDown(SDLK_w)) {
		_position.y += _speed;
		_direction = 2;
	}
	else if (_inputManager->isKeyDown(SDLK_s)) {
		_position.y -= _speed;
		_direction = 3;
	}

	if (_inputManager->isKeyDown(SDLK_a)) {
		_position.x -= _speed;
		_playerDirection = false;
		_direction = 1;
	}
	else if (_inputManager->isKeyDown(SDLK_d)) {
		_position.x += _speed;
		_playerDirection = true;
		_direction = 0;
	}

	collideWithLevel(levelData);

	//check if at teleport and move player if so
	if (_position.x < _leftTeleport.x) {
		_position = _rightTeleport;
	}
	else if (_position.x > _rightTeleport.x) {
		_position = _leftTeleport;
	}
}

void Player::draw(Bengine::SpriteBatch& spriteBatch) {

	//matrix for draw coords
	glm::vec4 destRect = glm::vec4(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);
	
	//UV rect same for all sprites
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);


	//animate player
	switch (_direction) {
	//facing right
	case 0: 
		if (_frameCounter < 2) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_72.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 2 && _frameCounter < 4) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_73.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 4 && _frameCounter < 6) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_74.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 6 && _frameCounter < 8) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_75.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 8 && _frameCounter < 10) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_74.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 10 && _frameCounter < 12) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_73.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 12) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_72.png").id;
			_frameCounter = 0;
		}
		break;
	
		
	//facing left
	case 1: 
		if (_frameCounter < 2) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_58.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 2 && _frameCounter < 4) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_57.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 4 && _frameCounter < 6) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_56.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 6 && _frameCounter < 8) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_55.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 8 && _frameCounter < 10) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_56.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 10 && _frameCounter < 12) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_57.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 12) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_58.png").id;
			_frameCounter = 0;
		}
		break;
	
	//facing up
	case 2:
		if (_frameCounter < 2) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_102.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 2 && _frameCounter < 4) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_103.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 4 && _frameCounter < 6) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_104.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 6 && _frameCounter < 8) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_105.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 8 && _frameCounter < 10) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_104.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 10 && _frameCounter < 12) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_103.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 12) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_102.png").id;
			_frameCounter = 0;
		}
		break;

	//facing down
	case 3:
		if (_frameCounter < 2) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_89.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 2 && _frameCounter < 4) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_90.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 4 && _frameCounter < 6) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_91.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 6 && _frameCounter < 8) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_92.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 8 && _frameCounter < 10) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_91.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 10 && _frameCounter < 12) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_90.png").id;
			_frameCounter++;
		}
		else if (_frameCounter >= 12) {
			_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_89.png").id;
			_frameCounter = 0;
		}
		break;

	default:
		_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_58.png").id;
		break;
	}
	
	spriteBatch.draw(destRect, uvRect, _textureID, 0.0f, _color);

}