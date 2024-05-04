#ifndef __EMITTER_HPP__
#define __EMITTER_HPP__

#include "core/ray.hpp"
#include "core/transform.hpp"
#include "textures/texture.hpp"
#include "samplers/sampler.hpp"

#include <memory>

enum EmitterType {
	AREA 		= 0,
	POINT 		= 1,
	SPOT 		= 2,
	DIRECTIONAL = 3,
};

class Emitter {
	public:
		Emitter(Color emission) : color(emission) {
			o2w = Transform();
		}
		Emitter(const Transform &o2w, Color emission) : 
			o2w(o2w), color(emission) {}
		virtual glm::vec3 li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf, float &dist) const = 0;
		virtual void preprocess(BoundingBox bbox) {};
		virtual bool isDelta() { return true; }
		virtual float pdf(const HitRecord &hr, const glm::vec3 &wi) const {
			return 0.0;
		}
		virtual glm::vec3 Le(const Ray &ray, float dist, float &pdf) const {
			return BLACK;
		}

		Color color;
	protected:
		Transform o2w;
		//std::shared_ptr<Texture> emission;
};

#endif
