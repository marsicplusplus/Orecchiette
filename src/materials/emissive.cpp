#include "materials/emissive.hpp"

namespace Mat{
	Emissive::Emissive(const Color &a):Material(a){}

	MaterialType Emissive::getType() const { 
		return MaterialType::EMISSIVE;
	}
}
