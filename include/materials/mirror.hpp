#include "materials/material.hpp"

namespace Mat{
	class Mirror : public Material{
		public:
			Mirror(const Color &c);
			Mirror();
			MaterialType getType() const;

			bool sample(std::shared_ptr<Sampler> &sampler, const Ray& in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &r) const;
			glm::vec3 brdf(const HitRecord &hr, glm::vec3 wi);
	};
}
