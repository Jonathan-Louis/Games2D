#pragma once

#include <string>
#include <vector>
#include <Bengine/SpriteBatch.h>
#include <Bengine/ResourceManager.h>

//const tile width set to 64 pixels(tiles are set to squares)
const int TILE_WIDTH = 64;


class Level
{
public:
	//load level
	Level(const std::string &fileName);

	~Level();

	void draw();

	//getters
	glm::vec2 getPlayerStartPos() { return _startPlayerPos; }
	const std::vector<glm::vec2>& getZombieStartPos() { return _zombieStartPosition; }
	const std::vector<std::string>& getLevelData() { return _levelData; }
	int getNumHumans() { return _numHumans; }
	const int getWidth() { return _levelData[0].size(); }
	const int getHeight() { return _levelData.size(); }

private:
	std::vector<std::string> _levelData;
	int _numHumans;

	//spriteBatch for rendering level
	Bengine::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPos;

	std::vector<glm::vec2> _zombieStartPosition;
};

