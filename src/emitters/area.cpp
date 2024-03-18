#include "emitters/area.hpp"

void Area::sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal) const {
	shape->sample(sampler, point, normal);
}

float Area::area() const {
	return shape->area();
}
