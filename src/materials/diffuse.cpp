#include "materials/diffuse.hpp"
#include <iostream>
namespace{
	glm::vec3 randomCosineDirection(std::shared_ptr<Sampler> sampler) {
		auto r0 = sampler->getSample();
		auto r1 = sampler->getSample();
		float r = sqrt(r0);
		float theta = 2.0f * PI * r1;
		float x = r * cosf(theta);
		float y = r * sinf(theta);
		return glm::vec3(x, y, sqrt(1.0f-r0));
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
		auto x = sampler->getSample() * 2.0f - 1.0f;
		auto y = sampler->getSample() * 2.0f - 1.0f;
		auto z = sampler->getSample() * 2.0f - 1.0f;
		glm::vec3 dir(x,y,z);
		if(glm::dot(hr.normal, dir) < 0) dir = -dir;

		reflectedRay.origin = hr.point + EPS*dir;
		reflectedRay.direction = dir;
		pdf = 1.0f;
		brdf = this->albedo / PI;
		return true;
#else
		auto direction = randomCosineDirection(sampler);
		//direction = glm::normalize(direction);
		direction = glm::normalize(tangent2World(direction, hr.normal));
		reflectedRay.origin = hr.point + EPS*direction;
		reflectedRay.direction = direction;
		//std::cout << direction.x << " | " << direction.y << " | " << direction.z << "\n";

		pdf = glm::dot(hr.normal, direction)* ONE_OVER_PI;
		brdf = this->albedo / PI;
		return true;
#endif
	}
}
