#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__

#include "core/ray.hpp"
#include "core/transform.hpp"

class Primitive {
	public:
		Primitive(const Transform &obj2world);
		bool hit(const Ray &ray, const HitRecord& hr);
	private:
		Transform obj2world;
};

#endif
