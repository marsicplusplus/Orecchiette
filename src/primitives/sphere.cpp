#include "primitives/sphere.hpp"

Sphere::Sphere(const Transform &o2w, float radius, int material) :
	Primitive(o2w, material),
	radius(radius),
	radiusSq(radius*radius),
	invRadius(1.0f/radius),
	center(o2w.getTranslation()){
	buildBBox();
	A = ((float)4.0f/(float)3.0f) * PI * radius * radius;
}


void Sphere::buildBBox(){
	localBB.min = {-this->radius, -this->radius, this->radius};
	localBB.max = {this->radius, this->radius, -this->radius};
}

bool Sphere::hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const {
	float a = glm::dot(ray.direction, ray.direction);
	glm::vec3 oc = ray.origin - center;
	float halfB = glm::dot(ray.direction, oc);
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

	glm::vec3 normal = (ray.at(root) - center) * invRadius;
	hr.t = root;
	hr.point = ray.at(root);
	hr.normal = normal;
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

	point = (this->obj2world.getMatrix() * glm::vec4(ret, 1.0f)) * this->radius;
	normal = (this->obj2world.getMatrix() * glm::vec4(ret, 0.0f));
}
