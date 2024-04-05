#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "core/defs.hpp"
#include "core/ray.hpp"
#include "samplers/sampler.hpp"
#include <memory>

namespace Mat{
enum MaterialType {
	EMISSIVE 	= 0,
	DIFFUSE 	= 1,
	MIRROR 		= 2,
};

class Material {
	public:
		Material() {}
		Material(const Color &c) : albedo(c) {}
		virtual MaterialType getType() const = 0;
		virtual bool sample(std::shared_ptr<Sampler> &sampler, const Ray& in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &r) const { return false;} ;
		virtual glm::vec3 brdf(const HitRecord &hr) {return BLACK;};
		Color albedo;
};
}
#endif // __MATERIAL_HPP__
