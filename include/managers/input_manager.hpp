#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "GLFW/glfw3.h"
#include <unordered_map>

class InputManager {
protected:
	InputManager() {}
	static InputManager* inputManager;

public:
	InputManager(InputManager& other) = delete;
	void operator=(const InputManager&) = delete;

	static InputManager* GetInstance() {
		if (inputManager == nullptr) {
			inputManager = new InputManager();
		}
		return inputManager;
	}
	
	static void destroy() {
		delete InputManager::inputManager;
	}

	void disable() {
		isEnabled = false;
	}
	void enable() {
		isEnabled = true;
	}

	bool isKeyDown(int key);
	void setIsKeyDown(int key, bool isDown);

private:	
	std::unordered_map<int, bool> keys;
	bool isEnabled;
};

#endif