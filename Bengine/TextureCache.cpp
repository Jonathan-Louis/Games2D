#include "TextureCache.h"
#include "ImageLoader.h"

#include<iostream>

namespace Bengine {

	TextureCache::TextureCache() {

	}


	TextureCache::~TextureCache() {

	}

	GLTexture TextureCache::getTexture(std::string texturePath) {

		//looks for existing texture in map
		auto mit = _textureMap.find(texturePath);

		//check if not in map
		if (mit == _textureMap.end()) {
			//load texture from PNG file
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			//insert new texture into the map
			_textureMap.insert(make_pair(texturePath, newTexture));

			//std::cout << "Loaded texture\n";

			return newTexture;
		}

		//return texture if found
		//std::cout << "Used cached texture!\n";
		return mit->second;



	}
}