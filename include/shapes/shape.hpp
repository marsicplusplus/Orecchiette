#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__

#include "core/ray.hpp"
#include "core/transform.hpp"
#include "samplers/sampler.hpp"
#include "core/defs.hpp"
#include <memory>

class Shape {
	public:
        Shape() : m_obj2World(Transform()) {}
		Shape(const Transform &obj2world) : m_obj2World(obj2world) {}
		virtual bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const = 0;
		virtual void sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal, float &pdf) const = 0;
		virtual float area() const = 0;
		virtual BoundingBox getBBox() const = 0;
		float pdf(HitRecord hr, const glm::vec3 &wi) const { return 1.0 / area(); }


    protected:
        Transform m_obj2World;
		BoundingBox m_bbox;
};

#endif
