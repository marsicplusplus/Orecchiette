#include "emitters/area.hpp"

glm::vec3 Area::sample(std::shared_ptr<Sampler> &sampler) const {
	return primitive->sample(sampler);
}
