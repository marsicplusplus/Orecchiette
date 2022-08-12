#include "primitives/rect.hpp"

Rect::Rect(const Transform &obj2world, int material, std::shared_ptr<Emitter> emitter) : 
	Primitive(obj2world, material, emitter){}

bool Rect::hit(const Ray &ray, const float tMin, const float tMax, HitRecord &hr) const {
	return false;
}
