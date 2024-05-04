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
	pdf *= (dist * dist) / cosT;
	if(cosT > 0.0f) {
    	return this->color;
	} else {
		return BLACK;
	}
}

glm::vec3 Area::Le(const Ray &ray, float dist, float &pdf) const{
	HitRecord hr;
	auto ret = BLACK;
	if(this->shape->hit(ray, EPS, dist - EPS, hr)){ 
		float cosT = glm::dot(hr.normal, -ray.direction);
		auto solidAngle = (cosT * this->area()) / (dist * dist);
		pdf = 1.0 / solidAngle;
		ret = cosT > 0.0 ? this->color : BLACK;
	} else {
		pdf = 0.0;
		ret = BLACK;
	}
	return ret;
}

float Area::pdf(const HitRecord &hr, const glm::vec3 &wi) const {
	return shape->pdf(hr, wi);
}