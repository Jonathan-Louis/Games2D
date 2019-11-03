#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <Bengine/SpriteBatch.h>
#include <Bengine/ResourceManager.h>

const float TILE_WIDTH = 25.0f;

class Level
{
public:
	//load in level
	Level(const std::string& fileName);

	~Level();

	//draw the level
	void draw();

	//getters
	glm::vec2 getPlayerStartPos() { return _startPlayerPos; }
	const std::vector<std::string>& getLevelData() { return _levelData; }
	const int getWidth() { return _levelData[0].size(); }
	const int getHeight() { return _levelData.size(); }
	const glm::vec2 getPelletsLoc(int i) { return _pelletPos[i]; }
	const glm::vec2 getBigPelletsLoc(int i) { return _bigPelletPos[i]; }
	const int getNumPellets() { return _pelletPos.size(); }
	const int getNumBigPellets() { return _bigPelletPos.size(); }
	const glm::vec2 getLeftTeleport() { return _leftTeleport; }
	const glm::vec2 getRightTeleport() { return _rightTeleport; }
	const glm::vec2 getGhostStart() { return _ghostPos; }

private:
	std::vector<std::string> _levelData;

	//spriteBatch for rendering level
	Bengine::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPos;
	glm::vec2 _ghostPos;

	std::vector<glm::vec2> _pelletPos;
	std::vector<glm::vec2> _bigPelletPos;

	glm::vec2 _leftTeleport;
	glm::vec2 _rightTeleport;
};

