#include "primitives/rect.hpp"

Rect::Rect(const Transform &obj2world, int material) : 
	Primitive(obj2world, material){}

bool Rect::hit(const Ray &ray, const float tMin, const float tMax, HitRecord &hr) const {
	return false;
}
