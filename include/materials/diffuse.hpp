#include "materials/material.hpp"

namespace Mat{
	class Diffuse : public Material{
		public:
			Diffuse(const Color &c);
			Diffuse();
			MaterialType getType() const;

			bool reflect(const Ray& in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &r, std::shared_ptr<Sampler> &sampler) const;
			glm::vec3 brdf(glm::vec3 wo, glm::vec3 wi);
	};
}
