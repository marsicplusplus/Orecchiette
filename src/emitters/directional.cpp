#include "emitters/directional.hpp"

glm::vec3 Directional::li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf, float &dist) const {
	wi = -m_dir;
	pdf = 1.0;
	vRay.origin = hr.point + EPS * wi;
	vRay.direction = wi;
	dist = 1e6;
    float sceneRadius = glm::distance2(m_sceneBBox.max, m_sceneBBox.min);
	return color;
}