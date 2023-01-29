#include "primitives/xyrect.hpp"
#include "glm/geometric.hpp"

XYRect::XYRect(const Transform &obj2world, int material) : 
	Primitive(obj2world, material){}

bool XYRect::hit(const Ray &ray, const float tMin, const float tMax, HitRecord &hr) const {
	auto transformedRay = ray.transformRay(obj2world.getInverse());
	glm::fvec3 normal(0.0f, 0.0f, 1.0f);
	
	float t = transformedRay.origin.z / transformedRay.direction.z;
	auto p = transformedRay.at(t);
	if (t >= EPS && t > tMin && t < tMax
		&& p.x <= 1.0f && p.x >= -1.0f
		&& p.y <= 1.0f && p.y >= -1.0f) {
		hr.materialIdx = this->material;
		hr.t = t;
		hr.point = obj2world.getMatrix() * glm::fvec4(p, 0.0f);
		hr.setFaceNormal(ray, obj2world.getTransposeInverse() * glm::fvec4(normal, 0.0f));
		return true;
	}

	return false;
}

void XYRect::sample(std::shared_ptr<Sampler>& sampler, glm::vec3& point, glm::vec3& normal) const {}

float XYRect::area() const {
	return abs(this->obj2world.getScale().x * this->obj2world.getScale().y * this->obj2world.getScale().z);
}

void XYRect::buildBBox() {
}

