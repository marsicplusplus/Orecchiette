#include "primitives/sphere.hpp"

Sphere::Sphere(const Transform &o2w, float radius, int material) :
	Primitive(o2w, material),
	radius(radius) {
	buildBBox();
	A = ((float)4.0f/(float)3.0f) * PI * radius * radius;
}


void Sphere::buildBBox(){
	localBB.min = {-this->radius, -this->radius, this->radius};
	localBB.max = {this->radius, this->radius, -this->radius};
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
	return true;
}

float Sphere::area() const {
	return A;
}

void Sphere::sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal) const {
	float u = sampler->getSample();
	float v = sampler->getSample();
	float theta = u * 2.0f * PI;
	float phi = acos(2.0 * v - 1.0);
	float r = cbrt(sampler->getSample());
	float sinTheta = sin(theta);
	float cosTheta = cos(theta);
	float sinPhi = sin(phi);
	float cosPhi = cos(phi);
	float x = r * sinPhi * cosTheta;
	float y = r * sinPhi * sinTheta;
	float z = r * cosPhi;
	glm::vec3 ret{x, y, z};

	point = (ret + this->obj2world.getTranslation()) * this->radius;
	normal = (ret);
}
