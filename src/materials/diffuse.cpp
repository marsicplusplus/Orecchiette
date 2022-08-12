#include "materials/diffuse.hpp"

namespace Mat{
	Diffuse::Diffuse(const Color &c){
		albedo = c;
	}
	Diffuse::Diffuse() {
		albedo = Color(1.0f);
	}

	MaterialType Diffuse::getType() const {
		return Mat::MaterialType::DIFFUSE;
	}

	bool Diffuse::reflect(const Ray& in, const HitRecord &r, Ray &reflectedRay) const { 
		reflectedRay.origin = r.point;
		reflectedRay.direction = in.direction - 2 * (glm::dot(in.direction, r.normal)) * r.normal;
		return true;
	}
}
