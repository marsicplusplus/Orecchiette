#ifndef __AREA_HPP__
#define __AREA_HPP__

#include "emitters/emitter.hpp"
#include "shapes/shape.hpp"

class Area : public Emitter {
	public:
		Area(const std::shared_ptr<Shape> &shape, const Color &c, float intensity = 10.0f)
			: Emitter(Transform(), c, intensity), shape(shape) {}

		virtual void sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal) const;
		virtual float area() const;

	private:
		const std::shared_ptr<Shape> shape;
};

#endif
