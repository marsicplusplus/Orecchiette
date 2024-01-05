#include "cameras/perspective.hpp"

namespace Camera {
Perspective::Perspective(glm::vec3 pos, glm::vec3 lookAt, const glm::vec2 &screenBounds, float fov) :
	origin(pos), screenBounds(screenBounds) {
	this->aspectRatio = screenBounds.x / screenBounds.y;
	float h = tanf(glm::radians(fov)*0.5f);
	this->sensorSize.y = 2.0f * h;
	this->sensorSize.x = this->sensorSize.y * this->aspectRatio;

	const glm::vec3 worldUp = glm::vec3{0.0f, 1.0f, 0.0f};
	this->forward = glm::normalize(lookAt);
	this->right = glm::cross(this->forward, worldUp);
	this->up = glm::cross(this->right, this->forward);

	origin = pos;
	this->horizontal = this->sensorSize.x * right;
	this->vertical = this->sensorSize.y * up;
	 this->llCorner = this->origin - this->horizontal / 2.0f - this->vertical / 2.0f - this->forward;
}


void Perspective::getCameraRay(float x, float y, Ray *ray, std::shared_ptr<Sampler> sampler) const{
	float jitterX = sampler->getSample();
	float jitterY = sampler->getSample();
	glm::vec2 uv = {
		float(x + jitterX) / (float)(this->screenBounds.x-1),
		float(y + jitterY) / (float)(this->screenBounds.y-1),
	};
	
	ray->direction = this->llCorner + uv.x * this->horizontal + uv.y * this->vertical - this->origin;
	ray->origin = this->origin;
}

void Perspective::update(float deltap) {

}
};
