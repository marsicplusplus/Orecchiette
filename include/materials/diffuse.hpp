#include "materials/material.hpp"

namespace Mat{
	class Diffuse : public Material{
		public:
			Diffuse(const Color &c);
			Diffuse();
			MaterialType getType() const;

			bool reflect(const Ray& in, const HitRecord &r, Ray &reflectedRay) const;

	};
}
