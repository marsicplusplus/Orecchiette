#include "materials/material.hpp"

namespace Mat {
	class Emissive : public Material { 
		public:
			Emissive(const Color &albedo = WHITE);
			MaterialType getType() const;
	};
};
