#include "primitives/triangle.hpp"

#include <algorithm>

TriangleMesh::TriangleMesh(const Transform &o2w, const std::string &name, unsigned int nTri, unsigned int nVerts, const unsigned int *vertexIndices, const glm::vec3 *P, const glm::vec3 *N, const glm::vec2 *UV) : 
	o2w(o2w), nTriangles{nTri}, nVertices{nVerts}, name{name},
	vertexIndices(vertexIndices, vertexIndices + 3 * nTri) {
	p.reset(new glm::vec3[nVertices]);
	for(int i = 0; i < nVertices; ++i){
		p[i] = P[i];
	}
	if(UV){
		uv.reset(new glm::vec2[nVertices]);
		for(int i = 0; i < nVertices; ++i){
			uv[i] = UV[i];
		}
	}
	if(N){
		n.reset(new glm::vec3[nVertices]);
		for(int i = 0; i < nVertices; ++i){
			n[i] = N[i];
		}
	}
}

Triangle::Triangle(const std::shared_ptr<TriangleMesh>& mesh, unsigned int triangleNumber, int material) :
	Primitive(Transform(), material),
	mesh(mesh) {
	vIdx = &mesh->vertexIndices[triangleNumber * 3];
	this->buildBBox();
	glm::fvec3* v0 = &(this->mesh->p.get())[vIdx[0]];
	glm::fvec3* v1 = &(this->mesh->p.get())[vIdx[1]];
	glm::fvec3* v2 = &(this->mesh->p.get())[vIdx[2]];
	glm::vec3 E1(*v1 - *v0);
	glm::vec3 E2(*v2 - *v0);
	auto tmp = glm::cross(E1, E2);
	A = glm::length(tmp) / 2.0f;
}

void Triangle::buildBBox() {
	glm::fvec3 *v0 = &(this->mesh->p.get())[vIdx[0]];
	glm::fvec3 *v1 = &(this->mesh->p.get())[vIdx[1]];
	glm::fvec3 *v2 = &(this->mesh->p.get())[vIdx[2]]; //wtf vIdx[2] = 4156242528 ???
	const glm::vec3 xVal = {v0->x, v1->x, v2->x};
	const glm::vec3 yVal = {v0->y, v1->y, v2->y};
	const glm::vec3 zVal = {v0->z, v1->z, v2->z};
	localBB.min.x = std::min(std::min(xVal.x, xVal.y), xVal.z);
	localBB.min.y = std::min(std::min(yVal.x, yVal.y), yVal.z);
	localBB.min.z = std::min(std::min(zVal.x, zVal.y), zVal.z);
	localBB.max.x = std::max(std::max(xVal.x, xVal.y), xVal.z);
	localBB.max.y = std::max(std::max(yVal.x, yVal.y), yVal.z);
	localBB.max.z = std::max(std::max(zVal.x, zVal.y), zVal.z);
	if(localBB.min.x == localBB.max.x){
		localBB.min.x -= 0.0001f;
		localBB.max.x += 0.0001f;
	}
	if(localBB.min.y == localBB.max.y){
		localBB.min.y -= 0.0001f;
		localBB.max.y += 0.0001f;
	}
	if(localBB.min.z == localBB.max.z){
		localBB.min.z -= 0.0001f;
		localBB.max.z += 0.0001f;
	}
}

bool Triangle::hit(const Ray& ray, const float tMin, const float tMax, HitRecord& rec) const {
	auto o2w = this->mesh->getTransform();
	auto transformInv = o2w.getInverse();
	auto transposeInv = o2w.getTransposeInverse();
	auto transformMat = o2w.getMatrix();
	const Ray transformedRay = ray.transformRay(transformInv);

	glm::fvec3 *v0 = &(this->mesh->p.get())[vIdx[0]];
	glm::fvec3 *v1 = &(this->mesh->p.get())[vIdx[1]];
	glm::fvec3 *v2 = &(this->mesh->p.get())[vIdx[2]];

	glm::fvec3 v0v1 = *v1 - *v0;
	glm::fvec3 v0v2 = *v2 - *v0;
	glm::fvec3 p = glm::cross(transformedRay.direction, v0v2);
	float det = glm::dot(v0v1, p);
	if (std::fabs(det) < EPS) return false;
	float inv = 1.0f / det;

	glm::fvec3 tv = transformedRay.origin - *v0;
	float u = glm::dot(tv, p) * inv;
	if (u < 0.0f || u > 1.0f) return false;

	glm::fvec3 q = glm::cross(tv, v0v1);
	float v = glm::dot(transformedRay.direction, q) * inv;
	if (v < 0.0f || u + v > 1.0f) return false;
	float tmp = glm::dot(v0v2, q) * inv;
	if (tmp < 0.0f) return false;

	if (tmp > tMin && tmp < tMax) {
		rec.t = tmp;
		auto localp = transformedRay.at(tmp);

		glm::fvec3 *n0 = &(this->mesh->n.get())[vIdx[0]];
		glm::fvec3 *n1 = &(this->mesh->n.get())[vIdx[1]];
		glm::fvec3 *n2 = &(this->mesh->n.get())[vIdx[2]];

		glm::fvec3 hitNormal;
		hitNormal = u * (*n1) + v * (*n2) + (1.0f - u - v) * (*n0);

		rec.setFaceNormal(ray, transposeInv * glm::fvec4(hitNormal, 0.0f));

		//glm::fvec2 uvs[3];
		//getUV(&uvs[0]);

		//glm::fvec2 uv = u * (uvs[1]) + v * (uvs[2]) + (1.0f - u - v) * (uvs[0]);
		//rec.uv = uv;
		rec.materialIdx = this->material;
		rec.point = transformMat * glm::fvec4(localp, 1.0f);
		rec.t = tmp;

		return true;
	}

	return false;
}

void Triangle::sample(std::shared_ptr<Sampler>& sampler, glm::vec3& point, glm::vec3& normal) const {
	glm::fvec3* v0 = &(this->mesh->p.get())[vIdx[0]];
	glm::fvec3* v1 = &(this->mesh->p.get())[vIdx[1]];
	glm::fvec3* v2 = &(this->mesh->p.get())[vIdx[2]];

	float p = sampler->getSample();
	float q = sampler->getSample();
	if (p + q > 1) {
		p = 1 - p;
		q = 1 - q;
	}

	// A + AB * p + BC * q
	float x = v0->x + (v1->x - v0->x) * p + (v2->x - v0->x) * q;
	float y = v0->y + (v1->y - v0->y) * p + (v2->y - v0->y) * q;
	float z = v0->z + (v1->z - v0->z) * p + (v2->z - v0->z) * q;
	auto o2w = this->mesh->getTransform();
	point = o2w.getMatrix() * glm::vec4{ x, y, z, 1.0f };
	if (this->mesh->n != nullptr) {
		glm::fvec3* n0 = &(this->mesh->n.get())[vIdx[0]];
		glm::fvec3* n1 = &(this->mesh->n.get())[vIdx[1]];
		glm::fvec3* n2 = &(this->mesh->n.get())[vIdx[2]];
		normal = *n0;
		float d00 = glm::dot(*v0, *v0);
		float d01 = glm::dot(*v0, *v1);
		float d11 = glm::dot(*v1, *v1);
		float d20 = glm::dot(*v2, *v0);
		float d21 = glm::dot(*v2, *v1);
		float denom = d00 * d11 - d01 * d01;
		float v = (d11 * d20 - d01 * d21) / denom;
		float w = (d00 * d21 - d01 * d20) / denom;
		float u = 1.0f - v - w;
		normal = (u * (*n1) + v * (*n2) + (1.0f - u - v) * (*n0));
		normal = o2w.getTransposeInverse() * glm::fvec4(normal, 0.0f);
	}
}

float Triangle::area() const {
	return A;
}