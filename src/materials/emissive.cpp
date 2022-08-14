#include "materials/emissive.hpp"

namespace Mat{
	Emissive::Emissive(){}

	MaterialType Emissive::getType() const { 
		return MaterialType::EMISSIVE;
	}
}
