#pragma once

#include <Bengine/ResourceManager.h>
#include <string>

#include "Agents.h"

class Pellets : public Agents
{
public:
	Pellets();
	~Pellets();

	//setters
	void setPos(glm::vec2 pos);
	//change texture for big pellets
	void setTexture() { _textureID = Bengine::ResourceManager::getTexture("Textures/rosekane_4.png").id; }

	//getters
	glm::vec4 getDestRect() { return _destRect; };

private:
	glm::vec4 _destRect;

};

