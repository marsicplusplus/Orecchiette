#include "shapes/sphere.hpp"

Sphere::Sphere(const Transform &o2w, float radius) : Shape(o2w),
													 radius(radius),
													 radiusSq(radius * radius),
													 invRadius(1.0f / radius)
{
	glm::vec3 center = m_obj2World.transformPoint(glm::vec3(0.0));
	m_bbox.min = center - radius;
	m_bbox.max = center + radius;
	A = 4.0f * PI * radiusSq;
}

bool solveQuadratic(const float &a, const float &b, const float &c, 
					float &x0, float &x1) {
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    
    return true;
}

bool Sphere::hit(const Ray &ray, const float tMin, const float tMax, HitRecord &hr) const
{
	auto r = m_obj2World.transformRay(ray);
	auto oc = 0.0f - r.origin;
	float a = glm::length2(r.direction);
	float h = glm::dot(r.direction, oc);
	auto c = glm::length2(oc) - radiusSq;

	auto disc = h * h - a * c;
	if(disc < 0) return false;
	auto sqrtd = sqrt(disc);
	auto root = (h - sqrtd) / a;
	if (root <= tMin || tMax <= root) {
		root = (h + sqrtd) / a;
		if (root <= tMin || tMax <= root) 
			return false;
	}
	auto local = r.at(root);
	hr.t = root;
	hr.point = ray.at(root);
	hr.setFaceNormal(ray, local * invRadius);
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
	glm::vec3 local = sampleUniformSphere(sampler);
	normal = glm::normalize(local);
	point = m_obj2World.transformPoint(radius * local);
	pdf = 1.0 / A;
}

BoundingBox Sphere::getBBox() const
{
	return m_bbox;
}