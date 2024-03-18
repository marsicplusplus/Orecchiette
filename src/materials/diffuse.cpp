#include "materials/diffuse.hpp"
#include "core/OrthonormalBasis.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#define UNIFORM_SAMPLING
namespace{
	glm::vec3 cosineSampling(const float &r1, const float &r2)
	{
		float phi = 2.0f * M_PI * r1;

		float x = cos(phi) * sqrt(r2);
		float y = sin(phi) * sqrt(r2);
		float z = sqrt(1.0 - r2);

		return glm::vec3(x, y, z);
	}

	glm::vec3 diffuseReflection(const HitRecord hr, std::shared_ptr<Sampler> &sampler)
	{
		auto sample = cosineSampling(sampler->getSample(), sampler->getSample());
		OrthonormalBasis onb;
		onb.buildFromNormal(hr.normal);
		return onb.local(sample);
	}
}

namespace Mat
{
	Diffuse::Diffuse(const Color &c) : Material(c) {}
	Diffuse::Diffuse() : Material(Color(0.5f)) {}

	MaterialType Diffuse::getType() const
	{
		return Mat::MaterialType::DIFFUSE;
	}

	bool Diffuse::reflect(const Ray &in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &hr, std::shared_ptr<Sampler> &sampler) const
	{
		brdf = this->albedo / PI;
		auto dir = glm::normalize(diffuseReflection(hr, sampler));
		if (glm::dot(hr.normal, dir) < 0)
			dir = -dir;
		reflectedRay.origin = hr.point + EPS * dir;
		reflectedRay.direction = dir;
		pdf = glm::dot(hr.normal, dir) / M_PI;
		return true;
	}

	glm::vec3 Diffuse::brdf(glm::vec3 wo, glm::vec3 wi)
	{
		return this->albedo / PI;
	}
}
