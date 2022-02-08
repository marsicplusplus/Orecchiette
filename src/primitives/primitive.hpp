#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__

#include "core/ray.hpp"
#include "core/transform.hpp"

class Primitive {
	public:
		Primitive(const Transform &obj2world, int material = 0) : obj2world(obj2world), material(material) {}
		virtual bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const = 0;
	protected:
		Transform obj2world;
		int material;
};

#endif
