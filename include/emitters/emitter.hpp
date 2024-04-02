#ifndef __EMITTER_HPP__
#define __EMITTER_HPP__

#include "core/ray.hpp"
#include "core/transform.hpp"
#include "textures/texture.hpp"
#include "samplers/sampler.hpp"

#include <memory>

class Emitter {
	public:
		Emitter(const Transform &o2w, Color emission) : 
			o2w(o2w), color(emission) {}
		virtual glm::vec3 li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf) const = 0;

		Color color;
	protected:
		Transform o2w;
		//std::shared_ptr<Texture> emission;
};

#endif
