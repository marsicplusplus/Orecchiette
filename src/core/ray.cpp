#include "core/ray.hpp"

Ray::Ray() : 
	origin{glm::vec3(0.0f)}, 
	direction{glm::vec3(0.0f, 0.0f, -1.0f)} {}

Ray::Ray(glm::vec3 origin, glm::vec3 direction) :
	origin{origin},
	direction{direction} {}

glm::vec3 Ray::at(float t) const {
	return origin + t * direction;
}

Ray Ray::transformRay(const glm::mat4 &transform) const {
	auto newDir = transform * glm::vec4(direction, 0.0f);
	auto newOg = transform * glm::vec4(origin, 1.0f);
	Ray ret(newOg, newDir);
	return ret;
}
