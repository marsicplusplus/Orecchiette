#ifndef __EMITTER_HPP__
#define __EMITTER_HPP__

#include "core/transform.hpp"
#include "textures/texture.hpp"
#include "samplers/sampler.hpp"

#include <memory>

class Emitter {
	public:
		//Emitter(const Transform &o2w, const std::shared_ptr<Texture> &emission) : 
			//o2w(o2w), emission(emission) {}
		Emitter(const Transform &o2w, Color emission) : 
			o2w(o2w), color(emission) {}
		virtual glm::vec3 sample(std::shared_ptr<Sampler> &sampler) const = 0;
		//virtual sample(const Ray &r);
		/* TODO: 
		 * We need:
		 * 	- method to sample the emitter;
		 * 	- method to get the pdf of the emitter;
		 * 	- eval light given a certain direction;
		 * 	- pdf given a certain direction;
		 *
		 * Take care for delta position and delta direction emitters (point light, directional lights);
		 * Each Emitter takes a texture that represents the Radiance? This way we can use the texture lights Jacco shown us;
		 * Maybe use indices to texture? This could create A LOT of additional complexity. Just use shared_ptr;
		 */

	protected:
		Transform o2w;
		Color color;
		//std::shared_ptr<Texture> emission;
};

#endif
