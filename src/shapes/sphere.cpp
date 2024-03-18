#include "shapes/sphere.hpp"

Sphere::Sphere(const Transform &o2w, float radius) :
	Shape(o2w),
	radius(radius),
	radiusSq(radius*radius),
	invRadius(1.0f/radius) {
	localBB.min = {-this->radius, -this->radius, this->radius};
	localBB.max = {this->radius, this->radius, -this->radius};
	A = 4.0f * PI * radius * radius;
}

bool Sphere::hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const {
	Ray transformedRay = ray.transformRay(obj2World.getInverse());
	float a = glm::dot(transformedRay.direction, transformedRay.direction);
	glm::vec3 oc = transformedRay.origin;
	float halfB = glm::dot(transformedRay.direction, oc);
	float c = glm::dot(oc, oc) - radiusSq;
	float discriminant = halfB * halfB - a * c;
	if (discriminant < 0.0f) {
		return false;
	}

	float sqrtDiscr = std::sqrt(discriminant);
	float root = (-halfB - sqrtDiscr) / a;
	if (root < tMin) {
		root = (-halfB + sqrtDiscr) / a;
	}

	if (root < EPS || root >= tMax) {
		return false;
	}

	glm::vec3 normal = (ray.at(root)) * invRadius;
	hr.t = root;
	hr.point = ray.at(root);
	hr.normal = normal;
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
	auto &t = this->obj2World.getTranslation();
	float x = t.x + this->radius * (r * sinPhi * cosTheta);
	float y = t.y + this->radius * (r * sinPhi * sinTheta);
	float z = t.z + this->radius * (r * cosPhi);
	glm::vec3 ret{x, y, z};

	point = (this->obj2World.getMatrix() * glm::vec4(ret, 1.0f)) * this->radius;
	normal = glm::normalize(-(ret - t)/this->radius);
}