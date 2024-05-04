#include "materials/material.hpp"

namespace Mat {
	class Emissive : public Material { 
		public:
			Emissive(const Color &albedo = WHITE);
			MaterialType getType() const;
			float pdf(const HitRecord &hr, const glm::vec3 &wi) const override {return 0.0;}
	};
};
