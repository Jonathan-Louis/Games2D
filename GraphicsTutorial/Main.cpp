#include <Windows.h>

#include "MainGame.h"

#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>

int main(int argc, char** argv) {
	
	MainGame mainGame;

	mainGame.run();
	
	return 0;
}