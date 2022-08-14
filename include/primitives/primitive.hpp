#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__

#include "core/ray.hpp"
#include "core/transform.hpp"
#include "emitters/emitter.hpp"

struct BoundingBox {
	glm::vec3 min = {INF, INF, INF};
	glm::vec3 max = {-INF, -INF, -INF};
};

/* TODO 
 * Maybe having something along the line of PBR? 
 * Shape class with intersection code for the shape and Geometry class with a pointer to the shape a material and the transform so we can use intersection?
 * Same for animation, we don't need to use StaticGeometry/AnimationGeometry/InstanceGeometry by using the same animation system of Tracey;
 * */
class Primitive {
	public:
		Primitive() : obj2world(Transform()), material(0) {}
		Primitive(const Transform &obj2world, int material = 0) : obj2world(obj2world), material(material){}
		virtual bool hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const = 0;
		virtual glm::vec3 sample(std::shared_ptr<Sampler> &sampler) const = 0;

		int material;

	protected:
		virtual void buildBBox() = 0;

		BoundingBox localBB;
		Transform obj2world;
};

#endif
