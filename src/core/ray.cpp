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
