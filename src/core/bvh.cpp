#include "core/bvh.hpp"

BVH::BVH() {
}

void BVH::buildBVH(const std::vector<std::shared_ptr<Primitive>> &primitives) {
    for(int i = 0; i < primitives.size(); i++) this->primIdxs.push_back(i);
    this->nodes = std::vector<BVHNode>(primitives.size() * 2 - 1);
    auto &root = nodes[this->rootNodeIdx];
    root.leftNode = 0;
    root.firstIdx = 0, root.primCount = primitives.size();
    updateNodeBounds(primitives, this->rootNodeIdx);
    subdivide(primitives, this->rootNodeIdx);
}

void BVH::updateNodeBounds(const std::vector<std::shared_ptr<Primitive>> &primitives, uint32_t idx) {
    auto& node = this->nodes[idx];
    for(uint32_t first = node.firstIdx, i = 0; i < node.primCount; ++i) {
        const auto& prim = primitives[first + i];
        node.nodeBounds.grow(prim->getBBox());
    }
}


bool BVH::intersectBVH(const Ray& ray, const std::vector<std::shared_ptr<Primitive>> &primitives, const uint32_t nodeIdx) const {
    auto& node = this->nodes[nodeIdx];
    if(!node.nodeBounds.intersect(ray)) return false;
    if(node.isLeaf()) {
        for(uint32_t i = 0; i < node.primCount; ++i) {
            primitives[this->primIdxs[node.firstIdx + i]]->hit(ray, 0, INF, hr);
        }
    } else {
        intersectBVH()
    }
}

void BVH::subdivide(const std::vector<std::shared_ptr<Primitive>> &primitives, uint32_t idx) {
    auto& node = this->nodes[idx];
    if(node.primCount <= 2) return;
    glm::vec3 extents = node.nodeBounds.max - node.nodeBounds.min;
    int axis = 0;
    if(extents.y > extents.x) axis = 1;
    if(extents.z > extents[axis]) axis = 2;
    float splitPos = node.nodeBounds.min[axis] + extents[axis] * 0.5;
    int i = node.firstIdx;
    int j = i + node.primCount - 1;
    while(i <= j) {
            if(primitives[this->primIdxs[i]]->centroid()[axis] < splitPos) i++;
            else std::swap(this->primIdxs[i], this->primIdxs[j--]);
    }
    int leftCount = i - node.firstIdx;
    if(leftCount == 0 || leftCount == node.primCount) return;
    int leftChildIdx = this->nodesUsed++;
    int rightChildIdx = this->nodesUsed++;
    node.leftNode = leftChildIdx;
    this->nodes[leftChildIdx].firstIdx = node.firstIdx;
    this->nodes[leftChildIdx].primCount = leftCount;
    this->nodes[rightChildIdx].firstIdx = i;
    this->nodes[rightChildIdx].primCount = node.primCount - leftCount;
    node.primCount = 0;

    updateNodeBounds(primitives, leftChildIdx);
    updateNodeBounds(primitives, rightChildIdx);

    subdivide(primitives, leftChildIdx);
    subdivide(primitives, rightChildIdx);
}