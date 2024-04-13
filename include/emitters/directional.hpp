#ifndef __DIRECTIONAL_HPP__
#define __DIRECTIONAL_HPP__

#include "emitters/emitter.hpp"

class Directional : public Emitter
{
public:
    Directional(glm::vec3 direction, Color c) : Emitter(c), m_dir(glm::normalize(-direction)) {}
    void preprocess(BoundingBox sceneBBox) { m_sceneBBox = sceneBBox; };
    glm::vec3 li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf, float &dist) const override;

private:
    glm::vec3 m_dir;
    BoundingBox m_sceneBBox;
};

#endif
