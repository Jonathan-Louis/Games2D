#pragma once

#include <Bengine/SpriteBatch.h>

#include "Agents.h"

//ghost sight radius to player
const float MAX_PLAYER_DISTANCE = 200.0f;

class Ghosts : public Agents
{
public:
	Ghosts(glm::vec2 pos, int ghostNum, int speed, glm::vec2 leftTeleport, glm::vec2 rightTeleport);
	~Ghosts();

	void update(glm::vec2 playerPos, const std::vector<std::string>& levelData);

	//draw ghoshts
	void draw(Bengine::SpriteBatch& spriteBatch);
private:

	glm::vec2 _direction;
	glm::vec2 _startLoc;
	int _frameCounter;
	int _ghostNum;
	glm::vec2 _leftTeleport;
	glm::vec2 _rightTeleport;
};

