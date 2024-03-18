#include "core/OrthonormalBasis.hpp"

OrthonormalBasis::OrthonormalBasis() {}

glm::vec3 OrthonormalBasis::local(const glm::vec3 &a) {
    return a.x * this->axis[0] + a.y * this->axis[1] + a.z * this->axis[2];
}

void OrthonormalBasis::buildFromNormal(const glm::vec3 n) {
    auto normalizedN = glm::normalize(n);
    glm::vec3 a = (abs(normalizedN.x) > 0.9) ? glm::vec3(0.0, 1.0, 0.0) : glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 v = glm::normalize(glm::cross(normalizedN, a));
    glm::vec3 u = glm::cross(normalizedN, v);
    this->axis[0] = u;
    this->axis[1] = v;
    this->axis[2] = normalizedN;
}