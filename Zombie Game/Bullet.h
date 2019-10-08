#pragma once

#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>
#include <vector>
#include <Bengine/ResourceManager.h>

class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed);
	~Bullet();

	//if update true, delete bullet
	bool update(const std::vector<std::string>& levelData);

	void draw(Bengine::SpriteBatch& spriteBatch);

	

	bool collideWithAgent(Agent* agent);

	//getters
	float getDamage() { return _damage; }

private:
	bool collideWithLevel(const std::vector<std::string>& levelData);

	float _damage;
	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;
	
};

