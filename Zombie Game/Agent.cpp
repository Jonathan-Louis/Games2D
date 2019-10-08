#include "Agent.h"
#include "Level.h"

#include <Bengine/ResourceManager.h>
#include <algorithm>

Agent::Agent() {

}

Agent::~Agent() {

}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	
	//vector of positions around current object
	std::vector<glm::vec2> collideTilePosition;

	//check four corners of current object
	//first corner
	checkTilePosition(levelData, collideTilePosition, _position.x, _position.y);

	//second corner
	checkTilePosition(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y);

	//third corner
	checkTilePosition(levelData, collideTilePosition, _position.x, _position.y + AGENT_WIDTH);

	//forth corner
	checkTilePosition(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);

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

//circular collision
bool Agent::collideWithAgent(Agent* agent) {

	const float MIN_DISTANCE = AGENT_RADIUS * 2;

	//position of current agent
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);

	//position of other agents
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {

		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;

		//push both agents apart if colliding
		_position += collisionDepthVec / 2.0f;
		agent->_position -= collisionDepthVec / 2.0f;

		return true;
	}
	
	return false;
}


void Agent::draw(Bengine::SpriteBatch& _spriteBatch) {
		
	static int textureID = Bengine::ResourceManager::getTexture("Textures/circle.png").id;

	//UV rect same for all sprites
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	_spriteBatch.draw(destRect, uvRect, textureID, 0, _color);
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float cornerX, float cornerY) {

	//check corner
	glm::vec2 cornerPos = glm::vec2(floor(cornerX / (float)TILE_WIDTH), floor(cornerY / (float)TILE_WIDTH));

	//if outside the world return
	if ((cornerPos.x < 0 || cornerPos.x >= levelData[0].size()) ||
		(cornerPos.y < 0 || cornerPos.y >= levelData.size())) {
		return;
	}

	if (levelData[cornerPos.y][cornerPos.x] != '*') {
		collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}

	
}

//Axis Alligned Bounding Box Collision
void Agent::collideWithTile(glm::vec2 tilePos) {

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

bool Agent::applyDamage(float damage) {
	_health -= damage;
	if (_health <= 0) {
		return true;
	}

	return false;
}