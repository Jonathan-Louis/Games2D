#include "SpriteBatch.h"

#include <algorithm>

namespace Bengine {

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) {

	}
	SpriteBatch::~SpriteBatch() {

	}

	//initialize vao and vbo
	void SpriteBatch::init() {
		createVertexArray();
	}

	//assigns sorting scheme to be used
	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
		_sortType = sortType;

		//emptying glyphs and renderbatch vectors, maintains allocated memory space to be reused
		for (int i = 0; i < _glyphs.size(); i++) {
			delete _glyphs[i];
		}
		_glyphs.clear();
		_renderBatches.clear();
	}


	//sorts glyphs to order to draw to screen
	void SpriteBatch::end() {
		sortGlyphs();
		createRenderBatches();
	}

	//creates glyphs to draw
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color) {
		Glyph* newGlyph = new Glyph;

		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		

		_glyphs.push_back(newGlyph);
		
	}

	//creates batches to render to screen
	void SpriteBatch::createRenderBatches() {
		
		//vector for storing vertices for each glyph, resized to size needed based on num of glyphs
		std::vector<vertex> vertices;
		vertices.resize(_glyphs.size() * 6);


		if (_glyphs.empty()) {
			return;
		}

		int currentVertex = 0;
		int offset = 0;

		//setting up first render batch
		_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);

		vertices[currentVertex++] = _glyphs[0]->topLeft;
		vertices[currentVertex++] = _glyphs[0]->bottomLeft;
		vertices[currentVertex++] = _glyphs[0]->bottomRight;
		vertices[currentVertex++] = _glyphs[0]->bottomRight;
		vertices[currentVertex++] = _glyphs[0]->topRight;
		vertices[currentVertex++] = _glyphs[0]->topLeft;
		offset += 6;

		//adding vertixes for each additional glyph
		for (int cg = 1 /*current glyph*/; cg < _glyphs.size(); cg++) {
			//checking if texture has changed to put glyphs into batches based on texture
			if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
			}
			else {
				_renderBatches.back().numVertices += 6;
			}
			vertices[currentVertex++] = _glyphs[cg]->topLeft;
			vertices[currentVertex++] = _glyphs[cg]->bottomLeft;
			vertices[currentVertex++] = _glyphs[cg]->bottomRight;
			vertices[currentVertex++] = _glyphs[cg]->bottomRight;
			vertices[currentVertex++] = _glyphs[cg]->topRight;
			vertices[currentVertex++] = _glyphs[cg]->topLeft;
			offset += 6;
		}

		//passing vertex data to buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orphans the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), nullptr, GL_DYNAMIC_DRAW);
		//uploads data to buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertex), vertices.data());


		//unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void SpriteBatch::createVertexArray() {

		//generate vertex array
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}

		//bind vertex array object
		glBindVertexArray(_vao);

		//generate buffers
		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}

		//bind buffer object
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

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

		//unbind vao
		glBindVertexArray(0);

	}

	//used to sort glyphs for order to draw to screen
	void SpriteBatch::sortGlyphs() {

		switch (_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
			break;
		}

	}

	//renders batches to screen
	void SpriteBatch::renderBatch() {

		//bind vertex buffer array
		glBindVertexArray(_vao);
		
		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}

		//unbind vertex buffer array
		glBindVertexArray(0);
	}

	//sorting by depth from foremost to back
	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
	//sorting by depth from rearmost to fron
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}
	//sorting using texture id's
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}

}