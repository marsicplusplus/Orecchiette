#include "materials/dielectric.hpp"
#include "core/OrthonormalBasis.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/norm.hpp"

namespace 
{
    float fresnelDielectric(const HitRecord &hr, glm::vec3 incident, float etaI, float etaT)
    {
        // Schlick
         if (!hr.frontFace)
        {
            std::swap(etaI, etaT);
        }
        float r0 = (etaI - etaT) / (etaI + etaT);
        r0 *= r0;
        float cosX = glm::dot(hr.normal, -incident);
        float n = etaI / etaT;
        if(etaI > etaT) {
            float sinT2 = n * n * (1.0 - cosX * cosX);
            if(sinT2 > 1.0f) { // TIR
                return 1.0f;
            }
            cosX = sqrt(1.0 - sinT2);
        }
       float x = 1.0 - cosX;
       return r0 + (1.0 - r0) * x * x * x * x * x;
    }
    glm::vec3 refractRay(const glm::vec3 &i, const glm::vec3 &n, float ratio) {
        float cosTheta = fmin(glm::dot(-i, n), 1.0);
        glm::vec3 rOut = ratio * (i + n * cosTheta);
        glm::vec3 rOutP = n * (float)-sqrt(fabs(1.0 - glm::length2(rOut)));
        return rOut + rOutP;
    }
}

namespace Mat
{
	Dielectric::Dielectric(const Color &c, float eta) : Material(c), mEta(eta) {}
	Dielectric::Dielectric() : Material(Color(0.5f)), mEta(1.5) {}

	MaterialType Dielectric::getType() const
	{
		return Mat::MaterialType::DIELECTRIC;
	}

	bool Dielectric::sample(std::shared_ptr<Sampler> &sampler, const Ray &in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &hr) const
	{
        float f = fresnelDielectric(hr, in.direction, in.currentEta, mEta);
        if(sampler->getSample() < f) {
            glm::vec3 wi = glm::reflect(in.direction, hr.normal);
            reflectedRay = Ray(hr.point + wi * 0.01f, wi) ;
            reflectedRay.currentEta = in.currentEta;
            reflectedRay.lastSpecular = true;
            pdf = f;
            brdf = f * this->albedo / glm::dot(hr.normal, wi); // Replace albedo with reflection albedo;
        } else {
            glm::vec3 ft = this->albedo * (1.0f - f);
            float ratio = 1.0;
            if(hr.frontFace) {
                ratio = reflectedRay.currentEta / mEta;
            }
            else {
                ratio = mEta / reflectedRay.currentEta;
            }
            glm::vec3 wi = glm::refract(in.direction, hr.normal, ratio);
            reflectedRay.origin = hr.point + wi * 0.01f;
            reflectedRay.direction = wi;
            reflectedRay.lastSpecular = true;
            pdf = 1.0f - f;
            brdf = ft / glm::dot(hr.normal, wi);
        }
        return true;
	}

	glm::vec3 Dielectric::brdf(const HitRecord &hr, const glm::vec3 &wi) const {
        return glm::vec3(0.0f);
	}

	float Dielectric::pdf(const HitRecord &hr, const glm::vec3 &wi) const {
        return 0.0;
    }
}
