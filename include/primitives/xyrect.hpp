#include "primitives/primitive.hpp"

class XYRect : public Primitive {
	public:
		XYRect(const Transform &obj2world, int material = 0);

		bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const;
		void sample(std::shared_ptr<Sampler>& sampler, glm::vec3& point, glm::vec3& normal) const;
		float area() const;

	private:
		void buildBBox();
};
