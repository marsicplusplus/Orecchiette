#include "primitives/sphere.hpp"

Sphere::Sphere(const Transform &o2w, float radius, int material) :
	Primitive(o2w, material),
	radius(radius) {
	}

bool Sphere::hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const {
	auto r = ray.transformRay(obj2world.getInverse());
	auto a = glm::length2(r.direction);
	auto half_b = dot(r.origin, r.direction);
	auto c = glm::length2(r.origin) - radius*radius;

	auto discriminant = half_b*half_b - a*c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	auto root = (-half_b - sqrtd) / a;
	if (root < tMin || tMax < root) {
		root = (-half_b + sqrtd) / a;
		if (root < tMin || tMax < root)
			return false;
	}

	hr.t = root;
	hr.point = obj2world.getMatrix() * glm::vec4(r.at(hr.t), 1.0f);
	hr.setFaceNormal(r, obj2world.getTransposeInverse() * glm::vec4((hr.point) / radius, 1.0f));
	hr.materialIdx = material;
	return false;
}
