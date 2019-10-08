#pragma once

#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>
#include <string>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

//including classes for human and zombie to avoid circular includes with header files
class Human;
class Zombie;

//class used for detecting collision between objects
class Agent
{
public:
	Agent();
	//virtual destructor to call child destructors as well
	virtual ~Agent();

	//pure virtual class to force update to all derived classes
	virtual void update(const std::vector<std::string>& levelData, 
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies) = 0;

	//check for each object collision with level walls
	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	void draw(Bengine::SpriteBatch& _spriteBatch);

	//return true if agent dies
	bool applyDamage(float damage);

	//getter for positions
	glm::vec2 getPosition() const { return _position; }

protected:

	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float cornerX, float cornerY);

	void collideWithTile(glm::vec2 tilePos);
	
	glm::vec2 _position;
	float _speed;
	Bengine::Color _color;
	float _health;
	
};

