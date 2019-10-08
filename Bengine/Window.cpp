#include "Window.h"
#include "Errors.h"

namespace Bengine {

	Window::Window() {

	}
	Window::~Window() {

	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		//checking window options passed in
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}

		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//creates window
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		//checking for errors with SDL window
		if (_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created!");
		}

		//set up OpenGL context
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);

		//checking for errors with glContext
		if (glContext == nullptr) {
			fatalError("SDL_GL context could not be created!");
		}

		//setting up GLEW and checking for errors
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialize GLEW!");
		}

		//checking OpenGL version on current machine
		printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

		//set background
		glClearColor(1.0f, 1.0f, 1.0f, 1.0);

		//turning V-Sync on
		SDL_GL_SetSwapInterval(1);

		//enable alpha blending on textures
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	//swapping between current buffers when drawing to the screen
	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_sdlWindow);
	}
}