#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"

namespace Bengine {

	GLTexture ImageLoader::loadPNG(std::string filePath) {

		//creates texture and initialize to 0
		GLTexture texture = {};

		//vectors to store in and out for decodePNG(picoPNG)
		std::vector<unsigned char> in;
		std::vector<unsigned char> out;

		unsigned long width, height;


		//loads in image file to buffer
		if (IOManager::readFileToBuffer(filePath, in) == false) {
			fatalError("Failed to load PNG file to buffer!");
		}

		//decode .png format into array of pixels
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());

		if (errorCode != 0) {
			fatalError("decodePNG failed with error: " + std::to_string(errorCode));
		}

		//genreate texture
		glGenTextures(1, &(texture.id));

		//bind texture to id
		glBindTexture(GL_TEXTURE_2D, texture.id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		//unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//set textures width and height
		texture.width = width;
		texture.height = height;

		//return texture data
		return texture;
	}
}