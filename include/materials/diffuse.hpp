#include "materials/material.hpp"

namespace Mat{
	class Diffuse : public Material{
		public:
			Diffuse(const Color &c);
			Diffuse();
			MaterialType getType() const;

			bool sample(std::shared_ptr<Sampler> &sampler, const Ray& in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &r) const;
			glm::vec3 brdf(const HitRecord &hr);
	};
}
