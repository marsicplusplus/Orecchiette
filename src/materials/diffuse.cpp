#include "materials/diffuse.hpp"

namespace Mat{
	Diffuse::Diffuse(const Color &c) : Material(c){}
	Diffuse::Diffuse() : Material(Color(0.5f)) {}

	MaterialType Diffuse::getType() const {
		return Mat::MaterialType::DIFFUSE;
	}

	bool Diffuse::reflect(const Ray& in, Ray &reflectedRay, const HitRecord &hr,  std::shared_ptr<Sampler> &sampler) const { 
		auto x = sampler->getSample() * 2.0f - 1.0f;
		auto y = sampler->getSample() * 2.0f - 1.0f;
		auto z = sampler->getSample() * 2.0f - 1.0f;
		glm::vec3 dir(x,y,z);
		if(glm::dot(hr.normal, dir) < 0) dir = -dir;

		reflectedRay.origin = hr.point + EPS*dir;
		reflectedRay.direction = dir;
		return true;
	}
}
