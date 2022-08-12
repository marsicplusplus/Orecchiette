#include "materials/material.hpp"
#include "primitives/primitive.hpp"

namespace Mat {
	class Emissive { 
		public:
		Emissive(const std::shared_ptr<Primitive> &shape);
		bool reflect(const Ray& in, const HitRecord &r, Ray &reflectedRay) const;
		MaterialType getType() const;

		std::shared_ptr<Primitive> shape;
	};
};
