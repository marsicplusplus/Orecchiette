#ifndef __AREA_HPP__
#define __AREA_HPP__

#include "emitters/emitter.hpp"
#include "primitives/primitive.hpp"

class Area : public Emitter {
	public:
		Area(const std::shared_ptr<Primitive> &area)
			: Emitter(Transform(), WHITE), primitive(area) {}

		glm::vec3 sample(std::shared_ptr<Sampler> &sampler) const ;

	private:
		const std::shared_ptr<Primitive> primitive;
};

#endif
