#pragma once

#include "shapes/shape.hpp"

#include <vector>
#include <string>

class TriangleMesh {
	public:
		TriangleMesh(Transform t, const std::string& name, unsigned int nTri, unsigned int nVerts, const unsigned int *vertexIndices, const glm::vec3 *p, const glm::vec3 *n, const glm::vec2 *uv);

	public:
		const unsigned int nTriangles, nVertices;
		std::vector<unsigned int> vertexIndices;
		std::unique_ptr<glm::vec3[]> p;
		std::unique_ptr<glm::vec3[]> n;
		std::unique_ptr<glm::vec2[]> uv;
		const std::string name;
		const Transform& getTransform() { return o2w; }

	private:
		Transform o2w;
};

class Triangle : public Shape {
	public:
		Triangle(const std::shared_ptr<TriangleMesh> &mesh, unsigned int triangleNumber);
		bool hit(const Ray& ray, const float tMin, const float tMax, HitRecord& rec) const override;
		float area() const override;
		void sample(std::shared_ptr<Sampler>& sampler, glm::vec3& point, glm::vec3& normal, float &pdf) const override;

	private:
		std::shared_ptr<TriangleMesh> mesh;
		const unsigned int *vIdx; // Store the first of the 3 indices of the triangle
		float A;
};
