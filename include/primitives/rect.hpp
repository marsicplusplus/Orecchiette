#include "primitives/primitive.hpp"

class Rect : public Primitive {
	public:
		Rect(const Transform &obj2world, int material = 0);
		bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const;

};
