#include "materials/emissive.hpp"

namespace Mat{
	Emissive::Emissive(const std::shared_ptr<Primitive> &shape){
		this->shape = shape;
	}

	bool Emissive::reflect(const Ray& in, const HitRecord &r, Ray &reflectedRay) const {
		return false;
	}

	MaterialType Emissive::getType() const { 
		return MaterialType::EMISSIVE;
	}
}
