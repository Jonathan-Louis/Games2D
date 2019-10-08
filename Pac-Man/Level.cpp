#include "Level.h"

#include <Bengine/Errors.h>
#include <fstream>
#include <iostream>

Level::Level(const std::string& fileName) {
	//open level text file
	std::ifstream file;
	file.open(fileName);

	//error checking when opening file
	if (file.fail()) {
		Bengine::fatalError("Failed to open " + fileName);
	}

	//reading in level data
	std::string temp;
	while (std::getline(file, temp)) {
		_levelData.push_back(temp);
	}

	//read in all tiles
	for (int i = 0; i < _levelData.size(); i++) {
		for (int j = 0; j < _levelData[i].size(); j++) {
			//get tile
			char tile = _levelData[i][j];

			//process tile
			switch (tile) {
				//case for walls
			case 'W':
				break;

				//set the pellets
			case '*':
				_pelletPos.emplace_back(glm::vec2(j, i));
				_levelData[i][j] = 'E';
				break;

				//set pacman starting point
			case 'P':
				_startPlayerPos.x = j * TILE_WIDTH;
				_startPlayerPos.y = i * TILE_WIDTH;
				_levelData[i][j] = 'E';
				break;
				
				//set ghosts starting point
			case 'G':
				_ghostPos.x = j * TILE_WIDTH;
				_ghostPos.y = i * TILE_WIDTH;
				_levelData[i][j] = 'E';
				break;

				//set left teleport space
			case 'A':
				_leftTeleport.x = j * TILE_WIDTH;
				_leftTeleport.y = i * TILE_WIDTH;
				_levelData[i][j] = 'E';
				break;

				//set right teleport
			case 'B':
				_rightTeleport.x = j * TILE_WIDTH;
				_rightTeleport.y = i * TILE_WIDTH;
				_levelData[i][j] = 'E';
				break;

				//ignore E(empty spaces) for level construction
			case 'E':
				break;

				//default to output if unkown char found in level data
			default:
				std::printf("Unexpected Symbol %c at (%d, %d)", tile, i, j);
				break;
			}
		}
	}
}


Level::~Level() {

}

void Level::draw() {

	_spriteBatch.init();
	_spriteBatch.begin();

	//texture coordinates and colors used for all tiles
	const glm::vec4 UVrect(0.0f, 0.0f, 1.0f, 1.0f);
	Bengine::Color whiteColor;

	//set full color saturation for tiles
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
			case 'W':
				_spriteBatch.draw(destRect, UVrect, Bengine::ResourceManager::getTexture("Textures/rosekane_148.png").id, 0.0f, whiteColor);
				break;

				//ignore E(empty spaces) for level construction
			case 'E':
				break;

				//default to output if unkown char found in level data
			default:
				std::printf("Unexpected Symbol %c at (%d, %d)", tile, i, j);
				break;
			}
		}
	}

	//close and draw level spritebatch
	_spriteBatch.end();
	_spriteBatch.renderBatch();
}