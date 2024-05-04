#ifndef __DIELECTRIC_HPP__
#define __DIELECTRIC_HPP__

#include "materials/material.hpp"

namespace Mat{
	class Dielectric : public Material{
		public:
			Dielectric(const Color &c, float eta = 1.5);
			Dielectric();
			MaterialType getType() const;

			bool sample(std::shared_ptr<Sampler> &sampler, const Ray& in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &r) const;
			glm::vec3 brdf(const HitRecord &hr, const glm::vec3 &wi) const override;
			float pdf(const HitRecord &hr, const glm::vec3 &wi) const override;
		private:
			float mEta;
	};
}
#endif