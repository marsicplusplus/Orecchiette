#include "core/scene.hpp"
#include "materials/diffuse.hpp"
#include "emitters/area.hpp"
#include <iostream>

Scene::Scene() {/*TODO empty/default scene?*/
	/* Create default empty material */
}
Scene::Scene(const std::string &fp) {/*TODO parse scene file? Maybe use the same json format of Tracey;*/}

int Scene::numberOfLights() const {
	return lights.size();
}

bool Scene::update(float dt) {
	bool ret = false;
	if(this->camera != nullptr) {
		ret = this->camera->update(dt);
	}
	return ret;
}

bool Scene::traverse(const Ray &ray, const float tMin, const float tMax, HitRecord &rec, std::shared_ptr<Sampler> sampler) {
	HitRecord tmp;
	tmp.point = {INF, INF, INF};
	bool hasHit = false;
	float closest = tMax;

	// for(const auto &p : primitives){
	for (auto i = 0; i < primitives.size(); ++i) {
		const auto& p = primitives.at(i);
		if (p->hit(ray, tMin, closest, tmp)) {
			hasHit = true;
			closest = tmp.t;
			rec = tmp;
			rec.geomIdx = i;
		}
	}
	return hasHit;
}

const std::shared_ptr<Emitter> Scene::sampleLight(std::shared_ptr<Sampler> &sampler, uint32_t idx, glm::vec3 &point, glm::vec3 &normal, float &area) {
	lights[idx]->sample(sampler, point, normal);
	area = lights[idx]->area();
	return lights[idx];
}

const std::shared_ptr<Emitter> Scene::sampleLights(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal, float &area) {
	float f = sampler->getSample();
	int idx = (f == 1.0 ? lights.size()-1 : f*lights.size());
	lights[idx]->sample(sampler, point, normal);
	area = lights[idx]->area();
	return lights[idx];
}

bool Scene::isOccluded(const Ray &ray, const std::shared_ptr<Emitter> &light, float tMax) const {
	HitRecord hr;
	for(const auto &p : primitives){
		if (p->light != nullptr && p->light == light) continue;
		else if (p->hit(ray, EPS, tMax, hr)) return true;
	}
	return false;
}

void Scene::addMaterial(const std::shared_ptr<Mat::Material> &m) {
	materials.emplace_back(m);
}

const std::shared_ptr<Mat::Material> Scene::getMaterial(const uint64_t idx) const {
	if(idx < 0 || idx >= this->materials.size()) return nullptr;
	return materials[idx];
}

const std::shared_ptr<Primitive> Scene::getPrimitive(const uint64_t idx) const {
	if(idx < 0 || idx >= this->primitives.size()) return nullptr;
	return primitives[idx];
}

void Scene::setCamera(const std::shared_ptr<Camera::Camera> &cam){
	this->camera = cam;
}

const std::shared_ptr<Camera::Camera> Scene::getCamera() const {
	return camera;
}

void Scene::addPrimitive(const std::shared_ptr<Primitive> &p){
	if(materials[p->material]->getType() == Mat::EMISSIVE){
		auto light = std::make_shared<Area>(p->shape, materials[p->material]->albedo, 5.0f);
		lights.push_back(light);
		p->light = light;
	}
	primitives.push_back(p);
}


