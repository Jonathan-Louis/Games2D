#include <SDL/SDL.h>
#include <GL/glew.h>

#include "Bengine.h"

namespace Bengine {
	int init() {
		//initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		//creates 2nd buffer to prevent flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}