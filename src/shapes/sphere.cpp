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

glm::vec3 sampleUniformSphere(std::shared_ptr<Sampler> &sampler) {
	float z = 1 - 2 * sampler->getSample();
	float r = sqrt(1 - sqrt(z));
	float phi = 2 * PI * sampler->getSample();
	return glm::vec3(
		r * cos(phi),
		r * sin(phi),
		z
	);
}

void Sphere::sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal, float &pdf) const {
	point = glm::vec3(0.0) + radius * sampleUniformSphere(sampler);
	point *= radius / glm::distance(point, glm::vec3(0.0));

	normal = normalize(obj2World.getMatrix() * glm::vec4(point, 0.0));
	point = obj2World.getMatrix() * glm::vec4(point, 1.0);
	pdf = 1.0 / area();
}