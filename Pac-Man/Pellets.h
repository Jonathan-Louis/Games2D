#pragma once

#include <Bengine/ResourceManager.h>

#include "Agents.h"

class Pellets : public Agents
{
public:
	Pellets();
	~Pellets();

	//setters
	void setPos(glm::vec2 pos);

	//getters
	glm::vec4 getDestRect() { return _destRect; };

private:
	glm::vec4 _destRect;

};

