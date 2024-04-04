#include "shapes/shape.hpp"

class XZRect : public Shape {
	public:
		XZRect(const Transform &obj2world);

		bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const;
		void sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal, float &pdf) const;
		float area() const;
	private:
		glm::vec3 minPoint = glm::vec3(-1.0f, 0.0f, -1.0f);
		glm::vec3 maxPoint = glm::vec3(1.0f, 0.0f, 1.0f);
};