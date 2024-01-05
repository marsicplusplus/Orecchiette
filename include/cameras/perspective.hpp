#ifndef __PERSPECTIVE_HPP__
#define __PERSPECTIVE_HPP__

#include "cameras/camera.hpp"

namespace Camera{
class Perspective : public Camera {
	public:
		Perspective(glm::vec3 pos, glm::vec3 lookAt, const glm::vec2 &screenBound, float fov);

		virtual void getCameraRay(float u, float v, Ray *ray, std::shared_ptr<Sampler> sampler) const override;
		virtual void update(float deltap) override;
	private:
		glm::vec3 origin;
		glm::vec3 lower_left_corner;
		glm::vec3 horizontal;
		glm::vec3 vertical;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 llCorner;
		glm::vec2 sensorSize;
		glm::vec2 screenBounds;
		float aspectRatio;
};
};

#endif // __PERSPECTIVE_HPP__
