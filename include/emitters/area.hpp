#ifndef __AREA_HPP__
#define __AREA_HPP__

#include "emitters/emitter.hpp"
#include "primitives/primitive.hpp"

class Area : public Emitter {
	public:
		Area(const std::shared_ptr<Primitive> &area, const Color &c, float intensity = 10.0f)
			: Emitter(Transform(), c, intensity), primitive(area) {}

		virtual void sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal) const;
		virtual float area() const;

	private:
		const std::shared_ptr<Primitive> primitive;
};

#endif
