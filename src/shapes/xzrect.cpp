#include "shapes/xzrect.hpp"
#include "glm/geometric.hpp"

XZRect::XZRect(const Transform &obj2world) : 
	Shape(obj2world){}

bool XZRect::hit(const Ray &ray, const float tMin, const float tMax, HitRecord &hr) const {
	auto transformedRay = ray.transformRay(obj2World.getInverse());
	glm::fvec3 normal(0.0f, 1.0f, 0.0f);
	
	float t = -transformedRay.origin.y / transformedRay.direction.y;
	auto p = transformedRay.at(t);
	if (t >= EPS && t > tMin && t < tMax
		&& p.x <= 0.5f && p.x >= -0.5f
		&& p.z <= 0.5f && p.z >= -0.5f) {
		hr.t = t;
		hr.point = ray.at(t);
		hr.setFaceNormal(ray, obj2World.getTransposeInverse() * glm::fvec4(normal, 0.0f));
		// hr.normal = normal;
		return true;
	}

	return false;
}

void XZRect::sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal, float &pdf) const {
	point = glm::vec3(
		sampler->getSample(),
		0.0,
		sampler->getSample()
	);
	point = obj2World.getMatrix() * glm::vec4(point, 1.0);
	normal = obj2World.getTransposeInverse() * glm::fvec4(normal, 0.0f);
	pdf = 1/area();
}

float XZRect::area() const {
	return abs(this->obj2World.getScale().x * this->obj2World.getScale().y * this->obj2World.getScale().z);
}