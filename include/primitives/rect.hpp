#include "primitives/primitive.hpp"

class Rect : public Primitive {
	public:
		Rect(const Transform &obj2world, int material = 0, std::shared_ptr<Emitter> emitter = nullptr);
		bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const;

};
