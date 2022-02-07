#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "primitives/primitive.hpp"
#include "lights/light_object.hpp"
#include "cameras/camera.hpp"

#include <string>
#include <memory>
#include <vector>

class Scene {
	public:
		/**
		 * Initialize an empty scene.
		 */
		Scene() {};

		/**
		 * Parse the scene from a file.
		 */
		Scene(std::string fp) {};

		/**
		 * Traverse the scene with _ray_ to find the closest intersection between [tMin, tMax]
		 *
		 * @param ray Ray that will be used to traverse the scene;
		 * @param sampler Sampler used to obtain random numbers;
		 * @return true if the ray intersects, false otherwhise.
		 */
		bool traverse(const Ray &ray, std::shared_ptr<Sampler> sampler) {return false;};

	private:
		std::vector<std::shared_ptr<Primitive>> primitives;
		std::vector<std::shared_ptr<LightObject>> lights;
		std::shared_ptr<Camera> camera;
};

#endif
