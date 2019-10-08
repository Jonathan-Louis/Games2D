#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

#include "Bullet.h"


class Gun
{
public:
	Gun(std::string name, int fireRate, float spread, int bulletsPerShot, float bulletSpeed, float bulletDamage);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& direction, const glm::vec2& pos, std::vector<Bullet>& bullets);

private:
	void fireGun(const glm::vec2& dir, const glm::vec2& pos, std::vector<Bullet>& bullets);

	//gun info storage
	std::string _name;
	int _fireRate;
	float _spread;
	int _bulletsPerShot;
	float _bulletSpeed;
	float _bulletDamage;
		
	int _frameCount;
};

