#include "shapes/shape.hpp"

class XYRect : public Shape {
	public:
		XYRect(const Transform &obj2world);

		bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const;
		void sample(std::shared_ptr<Sampler>& sampler, glm::vec3& point, glm::vec3& normal) const;
		float area() const;
};
