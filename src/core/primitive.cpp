#include "core/primitive.hpp"

bool Primitive::hit(const Ray &ray, const float tMin, const float tMax, HitRecord& hr) const {
    HitRecord tmp;
    if(this->shape->hit(ray, tMin, tMax, tmp)) {
        hr = tmp;
        hr.materialIdx = this->material;
        return true;
    }
    return false;
}

void Primitive::sample(std::shared_ptr<Sampler> &sampler, glm::vec3 &point, glm::vec3 &normal) const {
    this->shape->sample(sampler, point, normal);
}

float Primitive::area() const {
    return shape->area();
}