#include "ResourceManager.h"

namespace Bengine {

	TextureCache ResourceManager::_textureCache;

	//get texture from texture cache
	GLTexture ResourceManager::getTexture(std::string texturePath) {
		return _textureCache.getTexture(texturePath);
	}
}