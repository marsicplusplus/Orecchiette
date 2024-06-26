#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "core/ray.hpp"
#include "core/transform.hpp"
#include "samplers/sampler.hpp"

#include <memory>
namespace Camera {
class Camera {
	public:
		virtual void getCameraRay(float x, float y, Ray *ray, std::shared_ptr<Sampler> sampler) const = 0;
		virtual bool update(float deltaP) = 0;
	private:
};
}
#endif
