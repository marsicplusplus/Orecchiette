#include "cameras/perspective.hpp"

Perspective::Perspective(glm::vec3 pos, glm::vec3 lookAt, const glm::vec2 &screenBounds, float fov) :
	origin(pos), screenBounds(screenBounds) {
	this->aspectRatio = screenBounds.x / screenBounds.y;
	this->sensorSize.x = 2.0f * tanf(glm::radians(fov) / 2.0f);
	this->sensorSize.y = this->sensorSize.x / this->aspectRatio;

	const glm::vec3 worldUp = glm::vec3{0.0f, 1.0f, 0.0f};
	this->forward = glm::normalize(origin - lookAt);
	this->right = glm::cross(worldUp, this->forward);
	this->up = glm::cross(this->forward, this->right);

	origin = pos;
	this->horizontal = this->sensorSize.x * right;
	this->vertical = this->sensorSize.y * up;
	 this->llCorner = this->origin - this->horizontal / 2.0f - this->vertical / 2.0f - this->forward;
}


void Perspective::getCameraRay(float x, float y, Ray *ray, std::shared_ptr<Sampler> sampler) const{
	float jitterX = sampler->getSample();
	float jitterY = sampler->getSample();
	glm::vec2 uv = {
		double(x) / (this->screenBounds.x-1),
		double(y) / (this->screenBounds.y-1),
	};
	
	ray->direction = this->llCorner + uv.x * this->horizontal + uv.y * this->vertical - this->origin;
	ray->origin = this->origin;
}
