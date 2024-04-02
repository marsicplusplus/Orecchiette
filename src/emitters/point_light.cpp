#include "emitters/point_light.hpp"

glm::vec3 PointLight::li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf) const {
	wi = glm::normalize(pos - hr.point);
	pdf = 1.0;
	vRay.origin = hr.point + EPS * wi;
	vRay.direction = wi;
	return color / glm::distance2(pos, hr.point);
}