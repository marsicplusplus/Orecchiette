#include "emitters/spot_light.hpp"


glm::vec3 SpotLight::li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf, float &dist) const {
	wi = glm::normalize(m_pos - hr.point);
	pdf = 1.0;
	vRay.origin = hr.point + EPS * wi;
	vRay.direction = wi;

	float fallout;
	glm::vec3 wl = glm::normalize(o2w.getInverse() * glm::vec4(-wi, 1.0));
	float cosTheta = wl.z;
	if (cosTheta < m_cosTotalWidth)
		fallout = 0;
	if (cosTheta > m_cosFalloffStart)
		fallout = 1;
	else
	{
		float delta = (cosTheta - m_cosTotalWidth) / (m_cosFalloffStart - m_cosTotalWidth);
		float fallout = delta * delta * delta * delta;
	}
	dist = glm::distance2(m_pos, hr.point);

	return color * fallout / dist;
}