#ifndef __DEFS_HPP__
#define __DEFS_HPP__

#include "glm/vec3.hpp"
#include <limits>

const float INF 			= std::numeric_limits<float>::max();
const float EPS 			= 5e-6f;
const float PI 				= 3.14159265358979323846f;
const float ONE_OVER_PI 	= 1.0f/3.14159265358979323846f;
typedef glm::vec3 			Color;

#define DEG2RAD(_a) ((_a)*(PI)/180.0f)
#define RAD2DEG(_a) ((_a)*180.0f/ (PI) )

#endif // __DEFS_HPP__
