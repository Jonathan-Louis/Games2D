#include "Pellets.h"

Pellets::Pellets() {
	_textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_8.png").id;
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;
}

Pellets::~Pellets() {

}

void Pellets::setPos(glm::vec2 pos) {
	_position = pos;

	_position.x *= TILE_WIDTH;
	_position.y *= TILE_WIDTH;


	//create destRect for drawing
	_destRect.x = _position.x;
	_destRect.y = _position.y;
	_destRect.z = TILE_WIDTH;
	_destRect.w = TILE_WIDTH;
}