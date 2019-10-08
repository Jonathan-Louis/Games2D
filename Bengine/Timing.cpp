#include "Timing.h"

#include <SDL/SDL.h>


namespace Bengine {

	FpsLimiter::FpsLimiter() {

	}

	void FpsLimiter::init(float maxFps) {
		setMaxFps(maxFps);
	}

	void FpsLimiter::setMaxFps(float maxFps) {
		_maxFps = maxFps;
	}

	void FpsLimiter::begin() {
		_startTicks = SDL_GetTicks();
	}

	//end returns current fps
	float FpsLimiter::end() {

		//calculate fps
		calculateFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;
		//Limit the FPS to the max FPS
		if (1000.0f / _maxFps > frameTicks) {
			//delay to maxFPS
			SDL_Delay(1000.0f / _maxFps - frameTicks);
		}
		return _fps;
	}

	//calculates current fps
	void FpsLimiter::calculateFPS() {

		//creating array to store frame time samples to average for FPS
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;

		//getting previous frame time
		static float prevTicks = SDL_GetTicks();
		float currentTicks;

		//getting current frame time
		currentTicks = SDL_GetTicks();

		//calculating current frame time
		_frameTime = currentTicks - prevTicks;

		//storing current frame time
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		//set previous ticks to current ticks
		prevTicks = currentTicks;

		int count;

		//increment to next frame
		currentFrame++;

		//initialize for beginning count before going through NUM_SAMPLES interations
		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame;
		}
		else {
			count = NUM_SAMPLES;
		}

		//getting average FPS
		float frameTimeAverage = 0;

		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}

		frameTimeAverage /= count;

		//avoiding dividing by zero on first iteration
		if (frameTimeAverage > 0) {
			_fps = 1000.0f / frameTimeAverage;
		}
		else {
			_fps = 60.0f;
		}

	}
	

}