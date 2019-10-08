#include "IOManager.h"

#include <fstream>

namespace Bengine {

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {

		std::ifstream file(filePath, std::ios::binary);

		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		//seek to end of file
		file.seekg(0, std::ios::end);

		//get the file size
		int fileSize = file.tellg();

		//seek back to the beginning
		file.seekg(0, std::ios::beg);

		//ignore file header if in file
		fileSize -= file.tellg();

		//resize buffer to file size
		buffer.resize(fileSize);

		//read file
		file.read((char*) & (buffer[0]), fileSize);

		//close file
		file.close();
	}
}