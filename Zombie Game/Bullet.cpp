#include "Bullet.h"
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed) {
	_position = pos;
	_direction = dir;
	_damage = damage;
	_speed = speed;
}

Bullet::~Bullet() {

}

bool Bullet::update(const std::vector<std::string>& levelData) {

	_position += _direction * _speed;

	return(collideWithLevel(levelData));

}

void Bullet::draw(Bengine::SpriteBatch& _spriteBatch) {

	glm::vec4 destRect(_position.x, _position.y, BULLET_RADIUS * 2, BULLET_RADIUS * 2);

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	Bengine::Color color;

	color.r = 200;
	color.b = 0;
	color.g = 200;
	color.a = 205;

	_spriteBatch.draw(destRect, uvRect, Bengine::ResourceManager::getTexture("Textures/bullet.png").id, 0, color);
}

bool Bullet::collideWithAgent(Agent* agent) {

	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	//position of current agent
	glm::vec2 centerPosA = _position;

	//position of other agents
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	//if collision occurs return true
	if (collisionDepth > 0) {		
		return true;
	}

	return false;
}

bool Bullet::collideWithLevel(const std::vector<std::string>& levelData) {

	//bullet position in level
	glm::ivec2 gridPos;
	gridPos.x = floor(_position.x / TILE_WIDTH);
	gridPos.y = floor(_position.y / TILE_WIDTH);

	//if outside the world return true
	if ((gridPos.x < 0 || gridPos.x >= levelData[0].size()) ||
		(gridPos.y < 0 || gridPos.y >= levelData.size())) {
		return true;
	}

	//if hitting wall within level return true
	if (levelData[gridPos.y][gridPos.x] != '*') {
		return true;
	}

	return false;

}