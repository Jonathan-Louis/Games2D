#include "Gun.h"


Gun::Gun(std::string name, int fireRate, float spread, int bulletsPerShot, float bulletSpeed, float bulletDamage) {
	_name = name;
	_fireRate = fireRate;
	_spread = spread;
	_bulletsPerShot = bulletsPerShot;
	_bulletSpeed = bulletSpeed;
	_bulletDamage = bulletDamage;
	_frameCount = 0;
}

Gun::~Gun() {

}

void Gun::update(bool isMouseDown, const glm::vec2& direction, const glm::vec2& pos, std::vector<Bullet>& bullets) {
	_frameCount++;

	if (_frameCount >= _fireRate && isMouseDown) {		
		fireGun(direction, pos, bullets);
		_frameCount = 0;
	}

}

void Gun::fireGun(const glm::vec2& dir, const glm::vec2& pos, std::vector<Bullet>& bullets) {
	for (int i = 0; i < _bulletsPerShot; i++) {

		//random number generator
		static bool seeded = false;
		static std::mt19937 randomEngine;

		//prevent reseeding random engine each call
		if (!seeded) {
			randomEngine.seed(time(nullptr));
			seeded = true;
		}

		//random number for direction for humans
		std::uniform_real_distribution<float> randRotate(-_spread / 2, _spread / 2);


		bullets.emplace_back(pos, glm::rotate(dir, randRotate(randomEngine)), _bulletDamage, _bulletSpeed);
	}
}