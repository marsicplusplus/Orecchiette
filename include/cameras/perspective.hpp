#ifndef __PERSPECTIVE_HPP__
#define __PERSPECTIVE_HPP__

#include "cameras/camera.hpp"

namespace Camera{
class Perspective : public Camera {
	public:
		Perspective(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up, const glm::vec2 &screenBound, float fov);

		virtual void getCameraRay(float u, float v, Ray *ray, std::shared_ptr<Sampler> sampler) const override;

		void setPosition(glm::fvec3 pos);
		void setDirection(glm::fvec3 dir);
		void setFOV(float fov);

		inline float getFOV() const { return this->fov; };
		inline float getSensitivity() const { return this->sensitivity; };
		inline glm::vec3 getPosition() const { return this->position; }
		inline glm::vec3 getDirection() const { return this->direction; }

		virtual bool update(float deltap) override;
	private:
		void updateVectors();

		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 horizontal;
		glm::vec3 vertical;
		glm::vec3 llCorner;

		glm::vec2 screenBounds;

		glm::mat3x3 cameraMatrix;

		float fov;
		float aspectRatio;
		float viewportWidth;
		float viewportHeight;
		float sensitivity;

		const float speed = 1.0f;
};
};

#endif // __PERSPECTIVE_HPP__
