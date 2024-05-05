#ifndef __BVH_HPP__
#define __BVH_HPP__

#include "core/defs.hpp"
#include "core/primitive.hpp"

#include <vector>

struct BVHNode {
    BoundingBox nodeBounds;
    uint32_t leftNode, firstIdx, primCount;
    bool isLeaf() const { return primCount > 0; };
};

class BVH {
    public:
        BVH();
        void buildBVH(const std::vector<std::shared_ptr<Primitive>> &primitives);
        void updateNodeBounds(const std::vector<std::shared_ptr<Primitive>> &primitives, uint32_t idx);
        void subdivide(const std::vector<std::shared_ptr<Primitive>> &primitives, uint32_t idx);

        bool intersectBVH(const Ray& ray, const std::vector<std::shared_ptr<Primitive>> &primitives, const uint32_t nodeIdx) const;

    private:
        uint32_t rootNodeIdx = 0;
        uint32_t nodesUsed = 1;
        std::vector<BVHNode> nodes;
        std::vector<uint32_t> primIdxs;
};

#endif