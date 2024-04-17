#include "emitters/area.hpp"

float Area::area() const {
	return shape->area();
}

glm::vec3 Area::li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf, float &dist) const {
	glm::vec3 samplePoint, sampleNormal;
	shape->sample(sampler, samplePoint, sampleNormal, pdf);
	wi = glm::normalize(samplePoint - hr.point);
	vRay.origin = hr.point + EPS * wi;
	vRay.direction = wi;
	dist = glm::length(wi);
	auto solidAngle = (glm::dot(sampleNormal, -wi) * this->area()) / (dist * dist);
    return this->color * solidAngle;
}
