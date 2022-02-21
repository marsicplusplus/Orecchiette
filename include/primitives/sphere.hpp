#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "primitives/primitive.hpp"

class Sphere : public Primitive {
	public:
		Sphere(const Transform &o2w, float radius, int material = 0);
		virtual bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const override;

	private:
		void buildBBox() override;
		float radius;
};

#endif // __SPHERE_HPP__
