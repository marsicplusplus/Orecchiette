#ifndef __DEFS_HPP__
#define __DEFS_HPP__

#include "samplers/sampler.hpp"
#include "glm/vec3.hpp"
#include "glm/geometric.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

const float INF 			= std::numeric_limits<float>::max();
const float EPS 			= 1e-4;
const float PI 				= 3.14159265358979323846f;
const float ONE_OVER_PI		= 1.0f / 3.14159265358979323846f;
const float ONE_OVER_2PI	= 1.0f / (2.0f * 3.14159265358979323846f);
const float ONE_OVER_4PI	= 1.0f / (4.0f * 3.14159265358979323846f);
typedef glm::vec3 			Color;

const Color BLACK			= Color(0.0f, 0.0f, 0.0f);
const Color WHITE			= Color(1.0f, 1.0f, 1.0f);
const Color RED				= Color(1.0f, 0.0f, 0.0f);
const Color GREEN			= Color(0.0f, 1.0f, 0.0f);
const Color BLUE			= Color(0.0f, 0.0f, 1.0f);

#define DEG2RAD(_a) ((_a)*(PI)/180.0f)
#define RAD2DEG(_a) ((_a)*180.0f/ (PI) )

struct BoundingBox {
	glm::vec3 min = {INF, INF, INF};
	glm::vec3 max = {-INF, -INF, -INF};
	void grow(glm::vec3 &point) {
		min.x = std::min(point.x, min.x);
		min.y = std::min(point.y, min.y);
		min.z = std::min(point.z, min.z);
		max.x = std::max(point.x, max.x);
		max.y = std::max(point.y, max.y);
		max.z = std::max(point.z, max.z);
	}
	void grow(BoundingBox bbox) {
		min.x = std::min(bbox.min.x, min.x);
		min.y = std::min(bbox.min.y, min.y);
		min.z = std::min(bbox.min.z, min.z);
		max.x = std::max(bbox.max.x, max.x);
		max.y = std::max(bbox.max.y, max.y);
		max.z = std::max(bbox.max.z, max.z);

	}

};

#endif // __DEFS_HPP__
