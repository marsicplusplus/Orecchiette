#include "core/scene.hpp"
#include "materials/diffuse.hpp"
#include "emitters/area.hpp"
#include <iostream>

Scene::Scene() : m_bbox(nullptr), bvh(nullptr) {/*TODO empty/default scene?*/
	/* Create default empty material */
}
Scene::Scene(const std::string &fp) : m_bbox(nullptr), bvh(nullptr) {/*TODO parse scene file? Maybe use the same json format of Tracey;*/}

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

bool Scene::visibilityCheck(const Ray &ray, const float tMin, const float tMax, std::shared_ptr<Emitter> hitLight) {
	HitRecord tmp;
	tmp.point = {INF, INF, INF};

	for (auto i = 0; i < primitives.size(); ++i) {
		const auto& p = primitives.at(i);
		if(p->light == hitLight) continue;
		if (p->hit(ray, tMin, tMax, tmp)) {
			return false;
		}
	}
	return true;
}

void Scene::buildBVH() {
	if(this->bvh == nullptr) {
		std::cout << "Building the BVH" << std::endl;
		bvh = std::make_unique<BVH>();
		bvh->buildBVH(this->primitives);
	}
}

bool Scene::traverse(const Ray &ray, const float tMin, const float tMax, HitRecord &rec) {	
	HitRecord tmp;
	tmp.point = {INF, INF, INF};
	bool hasHit = false;
	float closest = tMax;

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

void Scene::addMaterial(const std::shared_ptr<Mat::Material> &m) {
	materials.emplace_back(m);
}

void Scene::addLight(const std::shared_ptr<Emitter> &m) {
	lights.emplace_back(m);
}

const std::shared_ptr<Mat::Material> Scene::getMaterial(const uint64_t idx) const {
	if(idx < 0 || idx >= this->materials.size()) return nullptr;
	return materials[idx];
}

const std::shared_ptr<Emitter> Scene::getEmitter(const uint64_t idx) const {
	if(idx < 0 || idx >= this->lights.size()) return nullptr;
	return lights[idx];
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
		auto light = std::make_shared<Area>(p->shape, materials[p->material]->albedo);
		lights.push_back(light);
		p->light = light;
	}
	primitives.push_back(p);
}

void Scene::preprocessLights() {
	if(m_bbox == nullptr) {
		m_bbox = new BoundingBox();
		for(const auto &p : primitives) {
			m_bbox->grow(p->getBBox());
		}
	}
	for(const auto &l : lights) {
		l->preprocess(*m_bbox);
	}

}

