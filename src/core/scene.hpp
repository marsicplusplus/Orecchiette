#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "primitives/primitive.hpp"
#include "emitters/emitter.hpp"
#include "cameras/camera.hpp"
#include "core/defs.hpp"
#include "textures/texture.hpp"

#include <string>
#include <memory>
#include <vector>

class Scene {
	public:
		/**
		 * Initialize an empty scene.
		 */
		Scene();

		/**
		 * Parse the scene from a file.
		 */
		Scene(const std::string &fp);

		/**
		 * Traverse the scene with _ray_ to find the closest intersection between [tMin, tMax]
		 *
		 * @param ray Ray that will be used to traverse the scene;
		 * @param tMin closest point on the ray direction;
		 * @param tMax farthest point on the ray direction;
		 * @param rec Hit Record to store information of the hit;
		 * @param sampler Sampler used to obtain random numbers;
		 * @return true if the ray intersects, false otherwhise.
		 */
		bool traverse(const Ray &ray, const float tMin, const float tMax, HitRecord &rec, std::shared_ptr<Sampler> sampler);

	private:
		std::vector<std::shared_ptr<Primitive>> primitives;
		std::vector<std::shared_ptr<Emitter>> lights;
		//std::vector<std::shared_ptr<Material>> materials;
		std::vector<std::shared_ptr<Texture>> textures;
		std::shared_ptr<Camera> camera;
};

#endif
