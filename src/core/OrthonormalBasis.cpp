#include "core/OrthonormalBasis.hpp"

OrthonormalBasis::OrthonormalBasis() {}

glm::vec3 OrthonormalBasis::local(const glm::vec3 &a) {
    return a.x * this->axis[0] + a.y * this->axis[1] + a.z * this->axis[2];
}

void OrthonormalBasis::buildFromNormal(const glm::vec3 n) {
    // auto hitNormal = glm::normalize(n);
    // if(std::fabs(hitNormal.x) > std::fabs(hitNormal.y)){
    //     this->axis[0] = glm::normalize(glm::vec3(hitNormal.z, 0, -hitNormal.x));
    // } else {
    //     this->axis[0] = glm::normalize(glm::vec3(0.0, -hitNormal.z, hitNormal.y));
    // }
    // this->axis[1] = glm::cross(hitNormal, this->axis[0]);
    // this->axis[2] = hitNormal; //n
    auto normalizedN = glm::normalize(n);
    glm::vec3 a = (abs(normalizedN.x) > 0.9) ? glm::vec3(0.0, 1.0, 0.0) : glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 v = glm::normalize(glm::cross(normalizedN, a));
    glm::vec3 u = glm::cross(normalizedN, v);
    this->axis[0] = u; // nt
    this->axis[1] = v; // nb
    this->axis[2] = normalizedN; // nb
}