#include "shapes/xzrect.hpp"
#include "glm/geometric.hpp"

XZRect::XZRect(const Transform &obj2world) : 
	Shape(obj2world){
		float halfX = 0.5;
		float halfZ = 0.5;
		glm::vec3 p1 = obj2world.transformPoint(glm::vec3(halfX, 0, halfZ));
		glm::vec3 p2 = obj2world.transformPoint(glm::vec3(halfX, 0, -halfZ));
		glm::vec3 p3 = obj2world.transformPoint(glm::vec3(-halfX, 0, halfZ));
		glm::vec3 p4 = obj2world.transformPoint(glm::vec3(-halfX, 0, -halfZ));
		m_bbox.grow(p1);
		m_bbox.grow(p2);
		m_bbox.grow(p3);
		m_bbox.grow(p4);
	}

bool XZRect::hit(const Ray &ray, const float tMin, const float tMax, HitRecord &hr) const {
	Ray transformedRay = m_obj2World.transformRay(ray);
	glm::fvec3 normal(0.0f, 1.0f, 0.0f);
	
	float t = -transformedRay.origin.y / transformedRay.direction.y;
	auto p = transformedRay.at(t);
	if (t >= EPS && t > tMin && t < tMax
		&& p.x <= maxPoint.x && p.x >= minPoint.x
		&& p.z <= maxPoint.z && p.z >= minPoint.z) {
		hr.t = t;
		hr.point = m_obj2World.transformPoint(transformedRay.at(t));
		hr.setFaceNormal(ray, m_obj2World.transformNormal(normal));
		return true;
	}

	return false;
}

void XZRect::sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal, float &pdf) const {
	point = glm::vec3(
		sampler->getSample() - 0.5,
		0.0,
		sampler->getSample() - 0.5
	);
	normal = glm::vec3(0.0f, 1.0f, 0.0f);
	point = m_obj2World.transformPoint(point);
	normal = m_obj2World.transformNormal(normal);
	pdf = 1/area();
}

float XZRect::area() const {
	return abs(this->m_obj2World.getScale().x * this->m_obj2World.getScale().y * this->m_obj2World.getScale().z);
}

BoundingBox XZRect::getBBox() const {
	return m_bbox;
}