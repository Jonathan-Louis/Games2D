#include "Camera2D.h"


namespace Bengine {

	//constructor
	Camera2D::Camera2D() :
		_position(0, 0),
		_cameraMatrix(1.0f),
		_scale(1.0f),
		_needsMatrixUpdate(true),
		_orthoMatrix(1.0f),
		_screenWidth(500),
		_screenHeight(500)
	{

	}

	//deconstructor
	Camera2D::~Camera2D() {

	}

	//initializing the camaera properties
	void Camera2D::init(int screenWidth, int screenHeight) {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}


	void Camera2D::update() {

		//checks if matrix needs to be updated
		if (_needsMatrixUpdate) {

			//camera translation
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			//camera scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			//reset camera update
			_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {

		//centers screen coords so center of screen is 0,0
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);

		//flip y coordinate so up is posite
		screenCoords = glm::vec2(screenCoords.x, -screenCoords.y);

		//adjusts screen coords based on the current scale factor
		screenCoords /= _scale;

		//translate with the current camera position
		screenCoords += _position;

		return screenCoords;
	}


}
