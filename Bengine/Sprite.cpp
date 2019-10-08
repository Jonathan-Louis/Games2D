#include "Sprite.h"
#include "vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace Bengine {

	//sprite initializer
	Sprite::Sprite() {
		_vboID = 0;
	}

	Sprite::~Sprite() {

		//delete buffer when class destoryed
		if (_vboID != 0) {
			glDeleteBuffers(1, &_vboID);
		}
	}

	//initializes sprite
	void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		//getting sprite texture
		_texture = ResourceManager::getTexture(texturePath);

		//generating buffer
		if (_vboID == 0) {
			glGenBuffers(1, &_vboID);
		}

		//array to store vertices for triangles
		vertex vertexData[6] = {};

		//set position and texture coor for each triangle
		//first triangle
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//second triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);



		//setting color data
		for (int i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 255, 255, 255);
		}

		//upload vertex data
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		//clearing buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//draws object
	void Sprite::draw() {

		//bind texture
		glBindTexture(GL_TEXTURE_2D, _texture.id);

		//binds buffer array and vbo
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		//sending array of positions
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//sets attribute info for position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));

		//sets attribute info for color
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), (void*)offsetof(vertex, color));

		//sets attribute for UV texture coor
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));

		//draws triangles
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//diable attribute array
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		//clearing buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
}