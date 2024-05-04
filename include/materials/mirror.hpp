#ifndef __MIRROR_HPP__
#define __MIRROR_HPP__
#include "materials/material.hpp"

namespace Mat{
	class Mirror : public Material{
		public:
			Mirror(const Color &c);
			Mirror();
			MaterialType getType() const;

			bool sample(std::shared_ptr<Sampler> &sampler, const Ray& in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &r) const;
			glm::vec3 brdf(const HitRecord &hr, const glm::vec3 &wi) const;
			float pdf(const HitRecord &hr, const glm::vec3 &wi) const override;
	};
}
#endif