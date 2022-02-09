#include "core/scene.hpp"

Scene::Scene() {/*TODO empty/default scene?*/}
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

const std::shared_ptr<Material> Scene::getMaterial(const uint64_t idx) const {
	if(idx < 0 || idx >= this->materials.size()) return nullptr;
	return materials[idx];
}

const std::shared_ptr<Camera> Scene::getCamera() const {
	return camera;
}
