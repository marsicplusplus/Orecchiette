#include "managers/input_manager.hpp"

InputManager* InputManager::inputManager = nullptr;

void InputManager::setIsKeyDown(int key, bool isDown) {
    if (inputManager == nullptr) {
        inputManager = new InputManager;
    }
    auto iterator = keys.find(key);
    if (iterator != keys.end()) {
        keys[key] = isDown;
    }
}

bool InputManager::isKeyDown(int key) {
    if (inputManager == nullptr) {
        inputManager = new InputManager;
    }
    bool res = false;
    if (isEnabled) {
        auto iterator = keys.find(key);
        if (iterator != keys.end()) {
            res = keys[key];
        }
    }
    return res;
}