#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "core/defs.hpp"
#include "core/ray.hpp"

namespace Mat{
enum MaterialType {
	EMISSIVE 	= 0,
	DIFFUSE 	= 1,
	MIRROR 		= 2,
};

class Material {
	public:
		Material() {}
		virtual MaterialType getType() const = 0;
		virtual bool reflect(const Ray& in, const HitRecord &r, Ray &reflectedRay) const = 0;

		Color albedo;
};
}
#endif // __MATERIAL_HPP__
