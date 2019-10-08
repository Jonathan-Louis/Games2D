#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>


namespace Bengine {
	//used for bitwise or to check for use of window options
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };


	//creates initial SDL window
	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		//gets window width and height
		int getScreenWidth() { _screenWidth; }
		int getScreenHeight() { _screenHeight; }

	private:
		SDL_Window* _sdlWindow;
		int _screenWidth;
		int _screenHeight;

	};
}

