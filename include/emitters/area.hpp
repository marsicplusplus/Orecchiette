#ifndef __AREA_HPP__
#define __AREA_HPP__

#include "emitters/emitter.hpp"
#include "shapes/shape.hpp"

class Area : public Emitter {
	public:
		Area(const std::shared_ptr<Shape> &shape, const Color &c)
			: Emitter(Transform(), c), shape(shape) {}

		glm::vec3 li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf) const override;
		virtual float area() const;

	private:
		const std::shared_ptr<Shape> shape;
};

#endif
