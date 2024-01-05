#include "cameras/perspective.hpp"
#include "managers/input_manager.hpp"
#include <glm/gtx/transform.hpp>

namespace Camera {
Perspective::Perspective(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up, const glm::vec2 &screenBounds, float fov) :
	position(pos), direction(lookAt), up(up), fov(glm::radians(fov)), screenBounds(screenBounds) {
	this->sensitivity = 5;
	this->aspectRatio = screenBounds.x / screenBounds.y;
	this->cameraMatrix = glm::mat3x3(1);
	updateVectors();
}


void Perspective::getCameraRay(float x, float y, Ray *ray, std::shared_ptr<Sampler> sampler) const{
	float jitterX = sampler->getSample();
	float jitterY = sampler->getSample();
	glm::vec2 uv = {
		float(x + jitterX) / (float)(this->screenBounds.x-1),
		float(y + jitterY) / (float)(this->screenBounds.y-1),
	};
	
	ray->direction = this->llCorner + uv.x * this->horizontal + uv.y * this->vertical - this->position;
	ray->origin = this->position;
}

void Perspective::setPosition(glm::vec3 pos) {
	this->position = pos;
	this->updateVectors();
}

void Perspective::setDirection(glm::vec3 dir) {
	if(dir != glm::vec3(0.0, 0.0, 0.0)) {
		this->direction = dir;
	} else {
		this->direction = glm::vec3(0.0, 0.0, -1.0);
	}
	this->updateVectors();
}

void Perspective::setFOV(float fov) {
	this->fov = glm::radians(fov);
	this->updateVectors();
}

void Perspective::updateVectors() {
	float h = tan(this->fov / 2.0f);
	this->viewportHeight = 2.0f * h;
	this->viewportWidth = this->aspectRatio * this->viewportHeight;

	// Redefine Up and Right as we allow for camera rotation
	this->direction = glm::normalize(this->direction);
	auto w = this->direction;
	this->right = glm::cross(w, glm::vec3(0, 1, 0));
	if (this->right == glm::vec3(0, 0, 0))
		this->right = glm::vec3(0, 0, 1);
	this->up = glm::cross(this->right, w);

	this->cameraMatrix = glm::mat3x3(this->right, this->up, this->direction);

	auto u = glm::normalize(glm::cross(w, this->up));
	auto v = glm::cross(u, w);
	this->horizontal = this->viewportWidth * u;
	this->vertical = this->viewportHeight * v;
	this->llCorner = this->position - this->horizontal / 2.0f - this->vertical / 2.0f + w;
}

bool Perspective::update(float dt) {
	bool updated = false;
	auto inputManager = InputManager::GetInstance();
	const float change = this->speed * dt;
	if (inputManager->isKeyDown(GLFW_KEY_W)) {
		this->position += this->direction * change;
		updated = true;
	}

	if (inputManager->isKeyDown(GLFW_KEY_S)) {
		this->position -= this->direction * change;
		updated = true;
	}

	if (inputManager->isKeyDown(GLFW_KEY_A)) {
		this->position -= this->right * change;
		updated = true;
	}

	if (inputManager->isKeyDown(GLFW_KEY_D)) {
		this->position += this->right * change;
		updated = true;
	}

	if (inputManager->isKeyDown(GLFW_KEY_Q)) {
		this->position += this->up * change;
		updated = true;
	}

	if (inputManager->isKeyDown(GLFW_KEY_E)) {
		this->position -= this->up * change;
		updated = true;
	}

	// if (inputManager->isKeyDown(GLFW_KEY_LEFT)) {
	// 	this->direction = glm::rotate(this->direction, -5.0f * dt, this->right);
	// 	updated = true;
	// }

	// if (inputManager->isKeyDown(GLFW_KEY_RIGHT)) {
	// 	this->direction = glm::rotate(this->direction, 5.0 * dt, this->right);
	// 	updated = true;
	// }

	if (updated) {
		updateVectors();
	}
	return updated;
}
};
