#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "shapes/shape.hpp"

class Sphere : public Shape {
	public:
		Sphere(const Transform &o2w, float radius);
		virtual bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const override;
		virtual void sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal, float &pdf) const override;
		virtual float area() const override;
		virtual BoundingBox getBBox() const override;

	private:
		float radius;
		float radiusSq;
		float invRadius;
		float A;
};

#endif // __SPHERE_HPP__
