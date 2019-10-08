#include "Bullet.h"

#include <Bengine/GLTexture.h>
#include <Bengine/ResourceManager.h>

//bullet constructor
Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime) {
	_position = pos;
	_direction = dir;
	_speed = speed;
	_lifeTime = lifeTime;
}

Bullet::~Bullet() {

}

//drawing bullets to screen
void Bullet::draw(Bengine::SpriteBatch& spriteBatch) {

	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Bengine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

//returns true if the bullets lifetime is over
bool Bullet::update() {

	//moving bullet on screen
	_position += _direction * _speed;

	//maintaining bullet lifetime
	_lifeTime--;
	if (_lifeTime == 0) {
		return true;
	}

	return false;
}
