#include "materials/diffuse.hpp"
#include "core/OrthonormalBasis.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#define UNIFORM_SAMPLING
namespace
{
	glm::vec3 cosineSampling(const float r1, const float r2)
	{
		float phi = 2.0f * PI * r1;

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

	bool Diffuse::sample(std::shared_ptr<Sampler> &sampler, const Ray &in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &hr) const
	{
		auto dir = glm::normalize(diffuseReflection(hr, sampler));
		reflectedRay.origin = hr.point + EPS * dir;
		reflectedRay.direction = dir;
		pdf = glm::dot(hr.normal, dir) * ONE_OVER_PI;
		brdf = this->albedo * ONE_OVER_PI;
		return true;
	}

	glm::vec3 Diffuse::brdf(const HitRecord &hr, const glm::vec3 &wi) const
	{
		return this->albedo * ONE_OVER_PI;
	}

	float Diffuse::pdf(const HitRecord &hr, const glm::vec3 &wi) const {
		return glm::dot(hr.normal, wi) * ONE_OVER_PI;
    }
}