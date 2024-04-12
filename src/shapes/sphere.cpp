#include "shapes/sphere.hpp"

Sphere::Sphere(const Transform &o2w, float radius) : Shape(o2w),
													 radius(radius),
													 radiusSq(radius * radius),
													 invRadius(1.0f / radius)
{
	glm::vec3 center = m_obj2World.transformPoint(glm::vec3(0.0));
	m_bbox.min = center - radius;
	m_bbox.max = center + radius;
	A = 4.0f * PI * radius * radius;
}

bool Sphere::hit(const Ray &ray, const float tMin, const float tMax, HitRecord &hr) const
{
	Ray transformedRay = ray.transformRay(m_obj2World.getInverse());
	float a = glm::dot(transformedRay.direction, transformedRay.direction);
	glm::vec3 oc = transformedRay.origin;
	float halfB = glm::dot(transformedRay.direction, oc);
	float c = glm::dot(oc, oc) - radiusSq;
	float discriminant = halfB * halfB - a * c;
	if (discriminant < 0.0f)
	{
		return false;
	}

	float sqrtDiscr = std::sqrt(discriminant);
	float root = (-halfB - sqrtDiscr) / a;
	if (root < tMin)
	{
		root = (-halfB + sqrtDiscr) / a;
	}

	if (root < EPS || root >= tMax)
	{
		return false;
	}

	glm::vec3 p = transformedRay.at(root);
	glm::vec3 normal = glm::normalize(p * invRadius);
	hr.t = root;
	hr.point = m_obj2World.transformPoint(p);
	normal = m_obj2World.transformNormal(normal);
	hr.setFaceNormal(ray, normal);
	return true;
}

float Sphere::area() const
{
	return A;
}

glm::vec3 sampleUniformSphere(std::shared_ptr<Sampler> &sampler)
{
	float z = 1 - 2 * sampler->getSample();
	float r = sqrt(std::max(0.0f, 1.0f - z * z));
	float phi = 2 * PI * sampler->getSample();
	return glm::vec3(
		r * cos(phi),
		r * sin(phi),
		z);
}

void Sphere::sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal, float &pdf) const
{
	point = radius * sampleUniformSphere(sampler);
	normal = glm::normalize(m_obj2World.transformNormal(point));
	point = m_obj2World.transformPoint(point);
	pdf = 1.0 / area();
}

BoundingBox Sphere::getBBox() const
{
	return m_bbox;
}