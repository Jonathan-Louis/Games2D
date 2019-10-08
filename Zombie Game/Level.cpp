#include "Level.h"

#include <Bengine/Errors.h>
#include <fstream>
#include <iostream>


Level::Level(const std::string& fileName) {

	//open level text file
	std::ifstream file;
	file.open(fileName);

	//error checking
	if (file.fail()) {
		Bengine::fatalError("Failed to open " + fileName);
	}

	//ignore first string in level file
	std::string temp;
	file >> temp >> _numHumans;

	//clear rest of first line
	std::getline(file, temp);


	//reading in level data
	while (std::getline(file, temp)) {
		_levelData.push_back(temp);
	}
	
	_spriteBatch.init();
	_spriteBatch.begin();

	//texture coordinates and colors used for all tiles
	glm::vec4 UVrect(0.0f, 0.0f, 1.0f, 1.0f);
	Bengine::Color whiteColor;

	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	//render all tiles
	for (int i = 0; i < _levelData.size(); i++) {
		for (int j = 0; j < _levelData[i].size(); j++) {
			//get tile
			char tile = _levelData[i][j];

			//get tile coordinates
			glm::vec4 destRect(j * TILE_WIDTH, i * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			//process tile
			switch (tile) {
				//case for drawing brick
				case 'B':
					_spriteBatch.draw(destRect, UVrect, Bengine::ResourceManager::getTexture("Textures/Brick.png").id, 0.0f, whiteColor);
					break;

				//set players starting point
				case '@':
					_startPlayerPos.x = j * TILE_WIDTH;
					_startPlayerPos.y = i * TILE_WIDTH;
					_levelData[i][j] = '*';
					break;

				//set zombies starting points
				case 'Z':
					_zombieStartPosition.emplace_back(j * TILE_WIDTH, i * TILE_WIDTH);
					_levelData[i][j] = '*';
					break;

				//ignore * for level construction
				case '*':
					break;

				//default to output if unkown char found in level data
				default:
					std::printf("Unexpected Symbol %c at (%d, %d)", tile, i, j);
					break;
			}
		}
	}

	_spriteBatch.end();
}

Level::~Level() {

}

void Level::draw() {
	_spriteBatch.renderBatch();
}