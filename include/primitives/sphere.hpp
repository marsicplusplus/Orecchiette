#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "primitives/primitive.hpp"

class Sphere : public Primitive {
	public:
		Sphere(const Transform &o2w, float radius, int material = 0);
		virtual bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const override;
		virtual void sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal) const override;
		virtual float area() const override;

	private:
		void buildBBox() override;
		float radius;
		float radiusSq;
		float invRadius;
		glm::vec3 center;
		float A;
};

#endif // __SPHERE_HPP__
