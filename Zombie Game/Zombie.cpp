#include "Zombie.h"
#include "Human.h"

Zombie::Zombie() {}

Zombie::~Zombie() {}

void Zombie::init(float speed, glm::vec2 pos) {
	_speed = speed;
	_position = pos;

	//zombie health
	_health = 100;

	//set zombies to green
	_color.r = 0;
	_color.g = 180;
	_color.b = 0;
	_color.a = 255;
}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies) {

	//get nearest human
	Human* closestHuman = getNearestHuman(humans);

	//direct zombie in direction of nearest human
	if (closestHuman != nullptr) {
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction * _speed;
	}

	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans) {

	Human* closestHuman = nullptr;
	float smallest = 999.0f;

	for (int i = 0; i < humans.size(); i++) {
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);

		if (distance < smallest) {
			smallest = distance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}