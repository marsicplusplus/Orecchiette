#include "core/scene.hpp"
#include "materials/diffuse.hpp"
#include "emitters/area.hpp"
#include <iostream>

Scene::Scene() {/*TODO empty/default scene?*/
	/* Create default empty material */
	materials.emplace_back(std::make_shared<Mat::Diffuse>());
}
Scene::Scene(const std::string &fp) {/*TODO parse scene file? Maybe use the same json format of Tracey;*/}

bool Scene::traverse(const Ray &ray, const float tMin, const float tMax, HitRecord &rec, std::shared_ptr<Sampler> sampler) {
	HitRecord tmp;
	tmp.point = {INF, INF, INF};
	bool hasHit = false;
	float closest = tMax;

	for(const auto &p : primitives){
		if (p->hit(ray, tMin, closest, tmp)) {
			hasHit = true;
			closest = tmp.t;
			rec = tmp;
		}
	}
	return hasHit;
}

bool Scene::sampleLights(const HitRecord &hr, std::shared_ptr<Sampler> sampler) {
	return false;
}

void Scene::addMaterial(const std::shared_ptr<Mat::Material> &m) {
	materials.emplace_back(m);
}

const std::shared_ptr<Mat::Material> Scene::getMaterial(const uint64_t idx) const {
	if(idx < 0 || idx >= this->materials.size()) return nullptr;
	return materials[idx];
}

void Scene::setCamera(const std::shared_ptr<Camera::Camera> &cam){
	this->camera = cam;
}

const std::shared_ptr<Camera::Camera> Scene::getCamera() const {
	return camera;
}

void Scene::addPrimitive(const std::shared_ptr<Primitive> &p){
	if(materials[p->material]->getType() == Mat::EMISSIVE){
		lights.push_back(std::make_shared<Area>(p));
	}
	primitives.push_back(p);
}


