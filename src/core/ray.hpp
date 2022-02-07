#ifndef __RAY_HPP__
#define __RAY_HPP__

#include <glm/geometric.hpp>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class Ray {
	public:
		Ray();
		Ray(glm::vec3 origin, glm::vec3 direction);
		glm::vec3 at(float t) const;

		glm::vec3 origin;
		glm::vec3 direction;
	private:
};

struct HitRecord{
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec2 uv;
	bool frontFace;
	float t;
	uint64_t materialIdx;

	inline void setFaceNormal(const Ray& r, const glm::vec3& outNormal) {
		frontFace = dot(r.direction, outNormal) < 0;
		normal = frontFace ? glm::normalize(outNormal) : glm::normalize(-outNormal);
	}
};


#endif //__RAY_HPP__
