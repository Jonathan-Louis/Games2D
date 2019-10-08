#pragma once

#include <Bengine/SpriteBatch.h>

#include "Agents.h"

class Ghosts : public Agents
{
public:
	Ghosts(glm::vec2 pos, int ghostNum, int speed);
	~Ghosts();

	void update(glm::vec2 playerPos, const std::vector<std::string>& levelData);

	//draw ghoshts
	void draw(Bengine::SpriteBatch& spriteBatch);
private:

	glm::vec2 _direction;
	glm::vec2 _startLoc;
	int _frameCounter;
	int _ghostNum;
};

