#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "shapes/shape.hpp"
#include "core/primitive.hpp"
#include "emitters/emitter.hpp"
#include "cameras/camera.hpp"
#include "core/defs.hpp"
#include "textures/texture.hpp"
#include "materials/material.hpp"

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
		 * @return true if the ray intersects, false otherwhise.
		 */
		bool traverse(const Ray &ray, const float tMin, const float tMax, HitRecord &rec);
		bool visibilityCheck(const Ray &ray, const float tMin, const float tMax, std::shared_ptr<Emitter> hitLight);

		const std::shared_ptr<Mat::Material> getMaterial(const uint64_t idx) const;
		const std::shared_ptr<Primitive> getPrimitive(const uint64_t idx) const;
		const std::shared_ptr<Emitter> getEmitter(const uint64_t idx) const;

		const std::shared_ptr<Camera::Camera> getCamera() const;
		void setCamera(const std::shared_ptr<Camera::Camera> &cam);

		void addPrimitive(const std::shared_ptr<Primitive> &p);
		void addMaterial(const std::shared_ptr<Mat::Material> &m);
		void addLight(const std::shared_ptr<Emitter> &e);
		int numberOfLights() const;

		bool update(float dt);
		void preprocessLights();

	private:
		std::vector<std::shared_ptr<Primitive>> primitives;
		std::vector<std::shared_ptr<Emitter>> lights;
		std::vector<std::shared_ptr<Mat::Material>> materials; 
		std::vector<std::shared_ptr<Texture>> textures;
		std::shared_ptr<Camera::Camera> camera;

		BoundingBox *m_bbox;
};

#endif
