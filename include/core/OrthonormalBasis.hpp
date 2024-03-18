#ifndef __ORTHONROMAL_BASIS_HPP__
#define __ORTHONROMAL_BASIS_HPP__

#include "glm/vec3.hpp"
#include "glm/glm.hpp"

class OrthonormalBasis{
    public:
        OrthonormalBasis();
        void buildFromNormal(glm::vec3 n);

        glm::vec3 local(const glm::vec3 &a);

        glm::vec3 axis[3];
};

#endif