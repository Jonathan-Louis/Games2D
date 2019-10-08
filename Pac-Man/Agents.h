#pragma once

#include <Bengine/Spritebatch.h>
#include <string>
#include <glm/glm.hpp>

#include "Level.h"

class Player;

const float AGENT_WIDTH = 20.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;
const float PELLET_RADIUS = 3.0f;


class Agents
{
public:
	Agents();
	~Agents();

	//draw that agents
	void draw(Bengine::SpriteBatch& spriteBatch, glm::vec4 destRect);

	//check agents location with level
	bool collideWithLevel(const std::vector<std::string>& levelData);

	//check for collision with other agents
	bool collideWithAgents(Agents* agent);

	//check for collision with pellets
	bool collideWithPellets(Agents* agent);

	//set agent position
	void setPosition(glm::vec2 pos) { _position = pos; }


	//getter for positions
	glm::vec2 getPosition() const { return _position; }

protected:
	//check tiles surrounding agent
	void checkTiles(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float cornerX, float cornerY);

	//collision with tiles
	void collideWithTile(glm::vec2 tilePos);
	
	float _speed;
	int _textureID;
	glm::vec2 _position;
	Bengine::Color _color;

};

