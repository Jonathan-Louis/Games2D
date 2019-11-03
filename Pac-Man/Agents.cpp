#include "Agents.h"


#include <algorithm>

Agents::Agents() {

}


Agents::~Agents() {

}


void Agents::draw(Bengine::SpriteBatch& spriteBatch, glm::vec4 destRect) {

	
	glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	spriteBatch.draw(destRect, uvRect, _textureID, 0.0f, _color);

}


bool Agents::collideWithLevel(const std::vector<std::string>& levelData) {
	//vector of positions around current object
	std::vector<glm::vec2> collideTilePosition;

	//check four corners of current object
	//first corner
	checkTiles(levelData, collideTilePosition, _position.x, _position.y);

	//second corner
	checkTiles(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y);

	//third corner
	checkTiles(levelData, collideTilePosition, _position.x, _position.y + AGENT_WIDTH);

	//forth corner
	checkTiles(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);

	if (collideTilePosition.size() == 0) {
		return false;
	}

	if (collideTilePosition.size() > 0) {
		//do the collision with tiles
		for (int i = 0; i < collideTilePosition.size(); i++) {
			collideWithTile(collideTilePosition[i]);
		}
	}

	return true;
}

bool Agents::collideWithAgents(Agents* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS * 2;

	//position of current agent
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);

	//position of other agents
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {

		return true;
	}

	return false;
}

bool Agents::collideWithPellets(Agents* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS + PELLET_RADIUS;

	//position of current agent
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);

	//position of other agents
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {

		return true;
	}

	return false;
}

bool Agents::collideWithBigPellets(Agents* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS + PELLET_RADIUS;

	//position of current agent
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);

	//position of other agents
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {

		return true;
	}

	return false;
}

void Agents::checkTiles(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float cornerX, float cornerY) {

	//check corner
	glm::vec2 cornerPos = glm::vec2(floor(cornerX / (float)TILE_WIDTH), floor(cornerY / (float)TILE_WIDTH));

	//if outside the world return
	if ((cornerPos.x < 0 || cornerPos.x >= levelData[0].size()) ||
		(cornerPos.y < 0 || cornerPos.y >= levelData.size())) {
		return;
	}

	if (levelData[cornerPos.y][cornerPos.x] != 'E') {
		collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}

}

//Axis Alligned Bounding Box Collision
void Agents::collideWithTile(glm::vec2 tilePos) {

	//constants for all collisions
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	//getting player position to check tiles
	glm::vec2 centerPlayerPosition = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPosition - tilePos;

	//checking how far into tile object is
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);


	//check for collision, if true collision occurs
	if (xDepth > 0 || yDepth > 0) {
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			//checking if player needs to move in the positive or negative direction
			if (distVec.x < 0) {
				_position.x -= xDepth;
			}
			else {
				_position.x += xDepth;
			}
		}
		else {
			if (distVec.y < 0) {
				_position.y -= yDepth;
			}
			else {
				_position.y += yDepth;
			}
		}
	}
}