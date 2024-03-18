#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__

#include "core/ray.hpp"
#include "core/transform.hpp"
#include "samplers/sampler.hpp"
#include "shapes/shape.hpp"
#include "core/defs.hpp"
#include "emitters/emitter.hpp"
#include <memory>

class Primitive {
	public:
		Primitive(std::shared_ptr<Shape> shape, int material, std::shared_ptr<Emitter> areaLight = nullptr) : material(material), shape(shape), light(areaLight) {}
		bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const;
		void sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal) const;
		float area() const;

		int material;
		std::shared_ptr<Emitter> light = 0;
		std::shared_ptr<Shape> shape = 0;
};

#endif
