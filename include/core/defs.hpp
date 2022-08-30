#ifndef __DEFS_HPP__
#define __DEFS_HPP__

#include "samplers/sampler.hpp"
#include "glm/vec3.hpp"
#include "glm/geometric.hpp"
#include <cmath>
#include <limits>

const float INF 			= std::numeric_limits<float>::max();
const float EPS 			= 5e-6f;
const float PI 				= 3.14159265358979323846f;
const float ONE_OVER_PI 	= 1.0f/3.14159265358979323846f;
typedef glm::vec3 			Color;

const Color BLACK			= Color(0.0f, 0.0f, 0.0f);
const Color WHITE			= Color(1.0f, 1.0f, 1.0f);
const Color RED				= Color(1.0f, 0.0f, 0.0f);
const Color GREEN			= Color(0.0f, 1.0f, 0.0f);
const Color BLUE			= Color(0.0f, 0.0f, 1.0f);

#define DEG2RAD(_a) ((_a)*(PI)/180.0f)
#define RAD2DEG(_a) ((_a)*180.0f/ (PI) )


#endif // __DEFS_HPP__
