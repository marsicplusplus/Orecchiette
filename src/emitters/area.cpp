#include "emitters/area.hpp"

float Area::area() const {
	return shape->area();
}

glm::vec3 Area::li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf) const {
	glm::vec3 samplePoint, sampleNormal;
	shape->sample(sampler, samplePoint, sampleNormal, pdf);
	wi = glm::normalize(samplePoint - hr.point);
	vRay = Ray(hr.point + EPS * wi, wi);
    return glm::dot(hr.normal, -wi) > 0.f ? this->color : BLACK;
}
