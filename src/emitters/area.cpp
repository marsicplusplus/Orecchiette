#include "emitters/area.hpp"

float Area::area() const {
	return shape->area();
}

glm::vec3 Area::li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf, float &dist) const {
	glm::vec3 samplePoint, sampleNormal;
	shape->sample(sampler, samplePoint, sampleNormal, pdf);
	wi = (samplePoint - hr.point);
	dist = glm::length(wi);
	wi = glm::normalize(wi);
	vRay.origin = hr.point + EPS * wi;
	vRay.direction = wi;
	float cosT = glm::dot(sampleNormal, -wi);
	auto solidAngle = (cosT * this->area()) / (dist * dist);
	if(cosT > 0.0f) {
    	return this->color * solidAngle;
	} else {
		return BLACK;
	}
}
