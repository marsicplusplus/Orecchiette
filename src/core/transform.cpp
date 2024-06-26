#include "core/transform.hpp"

Transform::Transform(){
	s = {1.0f, 1.0f, 1.0f};
	t = {0.0f, 0.0f, 0.0f};
	r = glm::angleAxis(0.0f, glm::vec3(1.0,0,0)) * glm::angleAxis(0.0f, glm::vec3(1.0,0,0)) * glm::angleAxis(0.0f, glm::vec3(1.0,0,0));
	m = glm::mat4(1.0f);
	mInv = glm::mat4(1.0f);
	mInvTran = glm::mat4(1.0f);
}

void Transform::scale(float uniform) {
	scale(uniform, uniform, uniform);
	updateMatrix();
}

void Transform::scale(glm::vec3 &sc) {
	scale(sc.x, sc.y, sc.z);
	updateMatrix();
}

void Transform::scale(float x, float y, float z){
	s.x *= x;
	s.y *= y;
	s.z *= z;
	updateMatrix();
}

void Transform::rotate(float angle, const glm::vec3 &rotAxis) {
	r = r * glm::angleAxis(angle, rotAxis);
	updateMatrix();
}

void Transform::translate(const glm::vec3 &tran) {
	translate(tran.x, tran.y, tran.z);
	updateMatrix();
}

void Transform::translate(float x, float y, float z) {
	t.x += x;
	t.y += y;
	t.z += z;
	updateMatrix();
}

glm::mat4 Transform::getMatrix() const {
	return m;
}
glm::mat4 Transform::getTransposeInverse() const {
	return mInvTran;
}
glm::mat4 Transform::getInverse() const {
	return mInv;
}

Transform Transform::lerp(const Transform *prev, const Transform *next, float dt){
	auto newT = prev->t * (1.f - dt) + next->t * dt;
	auto newS = prev->s * (1.f - dt) + next->s * dt;
	auto newR = glm::mix(prev->r, next->r, dt);
	Transform newTrans;
	newTrans.s = newS;
	newTrans.t = newT;
	newTrans.r = newR;
	newTrans.updateMatrix();
	return newTrans;
}

void Transform::updateMatrix(){
	m = glm::mat4{1.0f};
	m = glm::translate(m, t);
	m = m * glm::toMat4(r);
	m = glm::scale(m, s);

	mInv = glm::inverse(m);
	mInvTran = glm::transpose(mInv);
}


Ray Transform::transformRay(const Ray &ray) const
{
	return Ray(
		this->mInv * glm::vec4(ray.origin, 1.0),
		this->mInv * glm::vec4(ray.direction, 0.0)
	);
}

glm::vec3 Transform::transformPoint(const glm::vec3& p) const {
	return this->m * glm::vec4(p, 1.0);
}

glm::vec3 Transform::transformNormal(const glm::vec3& n) const {
	return this->mInvTran * glm::vec4(n, 0.0);
}