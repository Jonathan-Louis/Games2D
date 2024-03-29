#pragma once

#include "GLTexture.h"
#include <string>

namespace Bengine {
	//loads images into GLTextures
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}

