#include "materials/diffuse.hpp"
#include <iostream>
namespace{
	glm::vec3 randomCosineDirection(std::shared_ptr<Sampler> sampler) {
		auto r0 = sampler->getSample();
		auto r1 = sampler->getSample();
		float term1 = 2.0f * PI * r0;
		float term2 = sqrt(1.0f-r1);
		float x = term2 * cosf(term1);
		float y = term2 * sinf(term1);
		return glm::vec3(x, y, sqrt(r1));
	}

	void setupTangentSpace( const glm::vec3& N, glm::vec3& T, glm::vec3& B )
	{
		// "Building an Orthonormal Basis, Revisited"
		float sign = copysignf( 1.0f, N.z );
		const float a = -1.0f / (sign + N.z);
		const float b = N.x * N.y * a;
		B = glm::vec3( 1.0f + sign * N.x * N.x * a, sign * b, -sign * N.x );
		T = glm::vec3( b, sign + N.y * N.y * a, -N.y );
	}

	glm::vec3 tangent2World( const glm::vec3& V, const glm::vec3& N )
	{
		glm::vec3 T, B;
		setupTangentSpace( N, T, B );
		return V.x * T + V.y * B + V.z * N;
	}

	glm::vec3 tangent2World( const glm::vec3& V, const glm::vec3& N, const glm::vec3& T, const glm::vec3& B ){
		return V.x * T + V.y * B + V.z * N;
	}

}
namespace Mat{
	Diffuse::Diffuse(const Color &c) : Material(c){}
	Diffuse::Diffuse() : Material(Color(0.5f)) {}

	MaterialType Diffuse::getType() const {
		return Mat::MaterialType::DIFFUSE;
	}

	bool Diffuse::reflect(const Ray& in, Ray &reflectedRay, float &pdf, glm::vec3 &brdf, const HitRecord &hr,  std::shared_ptr<Sampler> &sampler) const { 
#if 1
		auto r0 = sampler->getSample();
		auto r1 = sampler->getSample();
		const float term1 = 2.0f * PI * r0, term2 = sqrtf( 1.0f - r1 * r1 );
		float s = cosf(term1), c = sinf(term1);
		glm::vec3 dir( c * term2, s * term2, r1 );
		dir = glm::normalize(tangent2World(dir, hr.normal));
		
		if(glm::dot(hr.normal, dir) < 0) dir = -dir;
		reflectedRay.origin = hr.point + EPS*dir;
		reflectedRay.direction = dir;
		pdf = 1.0f;
#else
		auto direction = randomCosineDirection(sampler);
		//direction = glm::normalize(direction);
		direction = glm::normalize(tangent2World(direction, hr.normal));
		reflectedRay.origin = hr.point + EPS*direction;
		reflectedRay.direction = direction;
		//std::cout << direction.x << " | " << direction.y << " | " << direction.z << "\n";

		pdf = std::max(0.0f, glm::dot(hr.normal, direction)) * ONE_OVER_PI;
		if (dot( hr.normal, direction ) <= 0) pdf = 0;
#endif
		brdf = this->albedo / PI;
		return true;
	}
}
