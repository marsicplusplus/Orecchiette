#ifndef __EMITTER_HPP__
#define __EMITTER_HPP__

#include "core/transform.hpp"
#include "textures/texture.hpp"
#include "samplers/sampler.hpp"

#include <memory>

class Emitter {
	public:
		Emitter(const Transform &o2w, Color emission, float intensity = 1.0f) : 
			o2w(o2w), color(emission), intensity(intensity) {}
		virtual void sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal) const = 0;
		virtual float area() const = 0;

		Color color;
		float intensity;
	protected:
		Transform o2w;
		//std::shared_ptr<Texture> emission;
};

#endif
