#include "Errors.h"

#include<cstdlib>
#include <iostream>
#include <SDL/SDL.h>


namespace Bengine {

	//prints out fatal errors if one occurs and quits out of SDL to close application
	void fatalError(std::string errorString) {

		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int temp;
		std::cin >> temp;
		SDL_Quit();
		exit(69);
	}
}