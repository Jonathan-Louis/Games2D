#include "Human.h"

#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>


Human::Human() :
	_frameCount(0)
{
}

Human::~Human() {}

void Human::init(float speed, glm::vec2 pos) {
		
	//random number generator for giving human random direction to travel
	static bool seeded = false;
	static std::mt19937 randomEngine;

	//prevent reseeding random engine each init call
	if (!seeded) {
		randomEngine.seed(time(nullptr));
		seeded = true;
	}

	static std::uniform_real_distribution<float> randDirection(-1.0f, 1.0f);

	_color.r = 200;
	_color.g = 0;
	_color.b = 200;
	_color.a = 200;

	_speed = speed;
	_position = pos;

	//human health
	_health = 20;

	//setting random direction
	_direction = glm::vec2(randDirection(randomEngine), randDirection(randomEngine));

	//checking to ensure humans are given a direction, if not setting direction
	if (_direction.length() == 0) {
		_direction = glm::vec2(1.0f, 0.0f);
	}

	//normalizing direction vector
	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies) {

	
	//random number generator
	static bool seeded = false;
	static std::mt19937 randomEngine;

	//prevent reseeding random engine each init call
	if (!seeded) {
		randomEngine.seed(time(nullptr));
		seeded = true;
	}

	//random number for direction for humans
	static std::uniform_real_distribution<float> randRotate(-0.25f, 0.25f);
	
	if (_frameCount >= 20) {
	
		//rotate human direction for random movement
		_direction = glm::rotate(_direction, randRotate(randomEngine));

		_frameCount = 0;
	}
	else {
		_frameCount++;
	}
	
	//if colliding with wall change direction
	if (collideWithLevel(levelData)) {
		//rotate human direction for random movement
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}


	_position += _speed * _direction;
	
}