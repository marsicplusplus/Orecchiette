#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__

#include "core/ray.hpp"
#include "core/transform.hpp"

/* TODO 
 * Maybe having something along the line of PBR? 
 * Shape class with intersection code for the shape and Geometry class with a pointer to the shape a material and the transform so we can use intersection?
 * Same for animation, we don't need to use StaticGeometry/AnimationGeometry/InstanceGeometry by using the same animation system of Tracey;
 * */
class Primitive {
	public:
		Primitive(const Transform &obj2world, int material = 0) : obj2world(obj2world), material(material) {}
		virtual bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const = 0;
	protected:
		Transform obj2world;
		int material;
};

#endif
