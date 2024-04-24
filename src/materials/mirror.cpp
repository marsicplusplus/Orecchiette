#include "materials/mirror.hpp"
#include "core/OrthonormalBasis.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

namespace Mat
{
	Mirror::Mirror(const Color &c) : Material(c) {}
	Mirror::Mirror() : Material(Color(0.5f)) {}

	MaterialType Mirror::getType() const
	{
		return Mat::MaterialType::MIRROR;
	}

	bool Mirror::sample(std::shared_ptr<Sampler> &sampler, const Ray &in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &hr) const
	{
        reflectedRay.direction = glm::reflect(in.direction, hr.normal);
        reflectedRay.origin = hr.point + reflectedRay.direction * 0.01f;
        brdf = this->albedo;
        pdf = 1.0;
        return true;
	}

	glm::vec3 Mirror::brdf(const HitRecord &hr, glm::vec3 wi)
	{
		return glm::vec3(0.0f);
	}
}
