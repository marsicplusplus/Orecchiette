#ifndef __AREA_HPP__
#define __AREA_HPP__

#include "emitters/emitter.hpp"
#include "shapes/shape.hpp"

class Area : public Emitter {
	public:
		Area(const std::shared_ptr<Shape> &shape, const Color &c)
			: Emitter(Transform(), c), shape(shape) {}

		glm::vec3 li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf, float &dist) const override;
		virtual float area() const;
		virtual bool isDelta() { return false; } 
		virtual float pdf(const HitRecord &hr, const glm::vec3 &wi) const override;
		virtual glm::vec3 Le(const Ray &ray, float dist, float &pdf) const override;

	private:
		const std::shared_ptr<Shape> shape;
};

#endif
