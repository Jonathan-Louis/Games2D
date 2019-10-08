#include "InputManager.h"


namespace Bengine {
	
	InputManager::InputManager() : _mouseCoords(0.0f, 0.0f) {

	}

	InputManager::~InputManager() {

	}

	void InputManager::pressKey(unsigned int keyID) {
		//sets key pressed to true
		//creates new space in map if first time pressing key
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID) {
		//sets key pressed to false when released
		_keyMap[keyID] = false;

	}

	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {
		//finds if keyID located in keyMap
		auto it = _keyMap.find(keyID);

		//if found returns current pressed state, if not found returns false by default
		if (it != _keyMap.end()) {
			return it->second;
		}
		else {
			return false;
		}
	}

}