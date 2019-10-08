#pragma once

namespace Bengine {

	class FpsLimiter {
	public:
		FpsLimiter();

		void init(float maxFps);

		void setMaxFps(float maxFps);

		void begin();

		float end();

	private:
		void calculateFPS();

		float _maxFps;
		float _startTicks;
		float _fps;
		float _frameTime;
	};
}