#include "shapes/triangle.hpp"

#include <algorithm>

TriangleMesh::TriangleMesh(Transform t, const std::string &name, unsigned int nTri, unsigned int nVerts, const unsigned int *vertexIndices, const glm::vec3 *P, const glm::vec3 *N, const glm::vec2 *UV) : 
	nTriangles{nTri}, nVertices{nVerts}, name{name}, o2w(t),
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

Triangle::Triangle(const std::shared_ptr<TriangleMesh>& mesh, unsigned int triangleNumber) :
	Shape(),
	mesh(mesh) {
	vIdx = &mesh->vertexIndices[triangleNumber * 3];
	glm::fvec3* v0 = &(this->mesh->p.get())[vIdx[0]];
	glm::fvec3* v1 = &(this->mesh->p.get())[vIdx[1]];
	glm::fvec3* v2 = &(this->mesh->p.get())[vIdx[2]];
	glm::vec3 E1(*v1 - *v0);
	glm::vec3 E2(*v2 - *v0);
	auto tmp = glm::cross(E1, E2);
	A = glm::length(tmp) / 2.0f;
}

// void Triangle::buildBBox() {
// 	glm::fvec3 *v0 = &(this->mesh->p.get())[vIdx[0]];
// 	glm::fvec3 *v1 = &(this->mesh->p.get())[vIdx[1]];
// 	glm::fvec3 *v2 = &(this->mesh->p.get())[vIdx[2]]; //wtf vIdx[2] = 4156242528 ???
// 	const glm::vec3 xVal = {v0->x, v1->x, v2->x};
// 	const glm::vec3 yVal = {v0->y, v1->y, v2->y};
// 	const glm::vec3 zVal = {v0->z, v1->z, v2->z};
// 	localBB.min.x = std::min(std::min(xVal.x, xVal.y), xVal.z);
// 	localBB.min.y = std::min(std::min(yVal.x, yVal.y), yVal.z);
// 	localBB.min.z = std::min(std::min(zVal.x, zVal.y), zVal.z);
// 	localBB.max.x = std::max(std::max(xVal.x, xVal.y), xVal.z);
// 	localBB.max.y = std::max(std::max(yVal.x, yVal.y), yVal.z);
// 	localBB.max.z = std::max(std::max(zVal.x, zVal.y), zVal.z);
// 	if(localBB.min.x == localBB.max.x){
// 		localBB.min.x -= 0.0001f;
// 		localBB.max.x += 0.0001f;
// 	}
// 	if(localBB.min.y == localBB.max.y){
// 		localBB.min.y -= 0.0001f;
// 		localBB.max.y += 0.0001f;
// 	}
// 	if(localBB.min.z == localBB.max.z){
// 		localBB.min.z -= 0.0001f;
// 		localBB.max.z += 0.0001f;
// 	}
// }

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
	float su0 = std::sqrt(p);
	float b0 = 1 - su0, b1 = q * su0;

	point = b0 * *v0 + b1 * *v1 + (1 - b0 - b1) * *v2;
	auto o2w = this->mesh->getTransform();
	point = o2w.getMatrix() * glm::vec4( point, 1.0f );
	if (this->mesh->n != nullptr && false) {
		glm::fvec3* n0 = &(this->mesh->n.get())[vIdx[0]];
		glm::fvec3* n1 = &(this->mesh->n.get())[vIdx[1]];
		glm::fvec3* n2 = &(this->mesh->n.get())[vIdx[2]];
		glm::vec3 n = b0 * *n0 + b1 * *n1 + (1 - b0 - b1) * *n2;
		normal = glm::normalize(n);
	} else {
		normal = glm::normalize(glm::vec3(
			glm::cross(*v1 - *v0, *v2 - *v0)
		));
	}
	normal = o2w.getTransposeInverse() * glm::fvec4(normal, 0.0f);
}

float Triangle::area() const {
	return A;
}